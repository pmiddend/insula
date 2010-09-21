#include "manager.hpp"
#include "parameters.hpp"
#include "instance.hpp"
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
#include "../height_map/vec2.hpp"
#include "../height_map/random_point.hpp"
#include "../height_map/object.hpp"
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
#include <fcppt/text.hpp>
#include <memory>
#include <cstddef>

insula::nugget::manager::manager(
	parameters const &params)
:
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
		params.sounds)
{
	physics::scalar const floating_height = 
		static_cast<physics::scalar>(
			sge::parse::json::find_member_exn<sge::parse::json::float_type>(
				params.json.members,
				FCPPT_TEXT("floating_height")));

	height_map::flatness_range const flatness =	
		json::parse_vector<height_map::scalar,2,sge::parse::json::float_type>(
			sge::parse::json::find_member_exn<sge::parse::json::array>(
				params.json.members,
				FCPPT_TEXT("flatness_range")));

	std::size_t const count = 
		static_cast<std::size_t>(
			sge::parse::json::find_member_exn<sge::parse::json::int_type>(
				params.json.members,
				FCPPT_TEXT("count")));

	random_engine rng_engine(
		random_seed());

	for (std::size_t i = 0; i < count; ++i)
	{
		height_map::vec2 const point2 = 
			height_map::random_point(
				params.height_map,
				params.water_level,
				rng_engine,
				flatness);

		nugget_positions_.push_back(
			physics::vec3(
				static_cast<physics::scalar>(
					point2.x()),
				params.height_map.project(
					point2) +
				backend_.model().bounding_box().bottom() + 
				floating_height,
				static_cast<physics::scalar>(
					point2.y())));
	}
}

insula::nugget::shared_instance_ptr const
insula::nugget::manager::instantiate(
	physics::world &w)
{
	return 
		std::make_shared<instance>(
			*this,
			w);
}
