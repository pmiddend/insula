#include "manager.hpp"
#include "parameters.hpp"
#include "../create_path.hpp"
#include "../random_engine.hpp"
#include "../random_seed.hpp"
#include "../math/box_radius.hpp"
#include "../model/object.hpp"
#include "../physics/shape_from_approximation.hpp"
#include "../physics/vec3.hpp"
#include "../physics/approximation/sphere.hpp"
#include "../physics/scalar.hpp"
#include "../height_map/flatness_range.hpp"
#include "../height_map/scalar.hpp"
#include "../height_map/vec3.hpp"
#include "../height_map/random_point.hpp"
#include "../height_map/object.hpp"
#include "../scene/manager.hpp"
#include "../physics/rigid/parameters.hpp"
#include "../physics/world.hpp"
#include "../json/parse_vector.hpp"
#include <sge/image/create_texture.hpp>
#include <sge/model/loader.hpp>
#include <sge/model/load_flags.hpp>
#include <sge/renderer/filter/trilinear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/float_type.hpp>
#include <sge/parse/json/int_type.hpp>
#include <sge/parse/json/string.hpp>
#include <sge/systems/instance.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/algorithm/ptr_container_erase.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert.hpp>
#include <memory>
#include <functional>
#include <cstddef>

insula::nugget::manager::manager(
	parameters const &params)
:
	world_(
		params.world),
	height_map_(
		params.height_map),
	backend_(
		// transparency is removed for now
		false,
		params.systems.renderer(),
		params.camera,
		params.model_shader,
		{
			{
				"texture",
				sge::image::create_texture(
					create_path(
						sge::parse::json::find_member_exn<sge::parse::json::string>(
							params.json.members,
							FCPPT_TEXT("texture")),
						FCPPT_TEXT("textures/props")),
					params.systems.renderer(),
					params.systems.image_loader(),
					sge::renderer::filter::trilinear,
					sge::renderer::resource_flags::none)
			}
		},
		std::make_shared<model::object>(
			params.systems.md3_loader()->load(
				create_path(
					sge::parse::json::find_member_exn<sge::parse::json::string>(
						params.json.members,
						FCPPT_TEXT("model")),
					FCPPT_TEXT("models/props")),
			sge::model::load_flags::switch_yz),
			params.systems.renderer())),
	shape_(
		physics::shape_from_approximation(
			physics::approximation::sphere(
				math::box_radius<graphics::scalar>(
					backend_.model().bounding_box())))),
	scene_manager_(
		params.scene_manager),
	broadphase_manager_(
		params.broadphase_manager),
	sounds_(
		params.sounds),
	rng_(
		random_seed()),
	floating_height_(
		static_cast<physics::scalar>(
			sge::parse::json::find_member_exn<sge::parse::json::float_type>(
				params.json.members,
				FCPPT_TEXT("floating_height")))),
	water_level_(
		params.water_level),
	flatness_range_(
		json::parse_vector<height_map::scalar,2,sge::parse::json::float_type>(
			sge::parse::json::find_member_exn<sge::parse::json::array>(
				params.json.members,
				FCPPT_TEXT("flatness_range")))),
	callback_connection_(
		world_.register_callback<physics::rigid::object,physics::rigid::object>(
			physics::object_type::nugget,
			physics::object_type::projectile,
			std::bind(
				&manager::contact_callback,
				this,
				std::placeholders::_1,
				std::placeholders::_2)))
{
	spawn();
}

void
insula::nugget::manager::update()
{
	BOOST_FOREACH(
		physics::rigid::object *ptr,
		to_delete_)
	{
		BOOST_FOREACH(
			rigid_model const &rm,
			models_)
		{
			if (&rm.physics_model() == ptr)
				fcppt::algorithm::ptr_container_erase(
					models_,
					&rm);
		}
	}
	to_delete_.clear();

	if (models_.empty())
		spawn();
}

insula::physics::vec3 const
insula::nugget::manager::closest_nugget(
	physics::vec3 const &ref) const
{
	FCPPT_ASSERT(!models_.empty());

	physics::scalar min_dist = 
		std::numeric_limits<physics::scalar>::max();
	physics::vec3 min;

	BOOST_FOREACH(
		rigid_model const &sm,
		models_)
	{
		physics::scalar const new_dist = 
			fcppt::math::vector::length(
				ref - sm.physics_model().position()); 

		if (new_dist < min_dist)
		{
			min_dist = new_dist;
			min = sm.physics_model().position();
		}
	}

	return min;
}

void
insula::nugget::manager::spawn()
{
	models_.push_back(
		new rigid_model(
			graphics::mat4::identity(),
			broadphase_manager_,
			physics::rigid::parameters(
				world_,
				physics::object_type::nugget,
				fcppt::math::vector::structure_cast<physics::vec3>(
					height_map::random_point(
						height_map_,
						water_level_,
						rng_,
						flatness_range_)
						+ height_map::vec3(
							0,
							static_cast<height_map::scalar>(
								backend_.model().bounding_box().bottom() 
									+ floating_height_),
							0)),
				// No rotation
				physics::mat3::identity(),
				shape_,
				physics::rigid::solidity::nonsolid,
				// Mass
				fcppt::optional<physics::scalar>(),
				// Linear velocity
				physics::vec3::null())));

	scene_manager_.insert(
		backend_,
		models_.back());
}

void
insula::nugget::manager::contact_callback(
	physics::rigid::object &nugget,
	physics::rigid::object &)
{
	to_delete_.insert(
		&nugget);
	sounds_.play(
		FCPPT_TEXT("score"));
}
