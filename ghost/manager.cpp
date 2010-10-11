#include "manager.hpp"
#include "manager_parameters.hpp"
#include "instance.hpp"
#include "../scene/manager.hpp"
#include "../physics/vec2.hpp"
#include "../physics/broadphase/manager.hpp"
#include "../physics/ghost_parameters.hpp"
#include "../physics/dim3.hpp"
#include "../physics/vec3.hpp"
#include "../stdlib/for_each.hpp"
#include "../math/uniform_scaling_matrix.hpp"
#include "../graphics/box.hpp"
#include "../graphics/vec3.hpp"
#include "../graphics/mat4.hpp"
#include "../graphics/dim3.hpp"
#include "../create_path.hpp"
#include "../json/parse_vector.hpp"
#include "../model/object.hpp"
#include "../model/backend.hpp"
#include "../model/shared_object_ptr.hpp"
#include "../height_map/flatness_range.hpp"
#include "../height_map/vec2.hpp"
#include "../height_map/random_point.hpp"
#include "../height_map/object.hpp"
#include "../random_engine.hpp"
#include "../random_seed.hpp"
#include "../math/box_radius.hpp"
#include <sge/model/loader.hpp>
#include <sge/model/load_flags.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/get.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/string.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/image/create_texture.hpp>
#include <sge/renderer/filter/trilinear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/systems/instance.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/matrix/rotation_axis.hpp>
#include <fcppt/math/twopi.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/assert.hpp>
#include <functional>
#include <memory>
#include <algorithm>

insula::ghost::manager::manager(
	manager_parameters const &params)
:
	scene_manager_(
		params.scene_manager),
	broadphase_(
		params.broadphase_manager.create())
{
	stdlib::for_each(
		params.array.elements,
		[&params,this](sge::parse::json::value const &v)
		{
			this->parse_single(
				params,
				sge::parse::json::get<sge::parse::json::object>(
					v));
		});
}

insula::ghost::manager::~manager()
{
}

void
insula::ghost::manager::parse_single(
	manager_parameters const &params,
	sge::parse::json::object const &o)
{
	model::shared_object_ptr model = 
		std::make_shared<model::object>(
			params.systems.md3_loader()->load(
				create_path(
					sge::parse::json::find_member_exn<sge::parse::json::string>(
						o.members,
						FCPPT_TEXT("model")),
					FCPPT_TEXT("models/props")),
				sge::model::load_flags::switch_yz),
			params.systems.renderer());

	// This is the same as with nuggets and props
	backends_.push_back(
		new model::backend(
			params.scene_manager,
			// ghosts are not transparent right now
			false,
			params.systems.renderer(),
			params.camera,
			params.model_shader,
			params.model_shadow_shader,
			{
				{
					"texture",
					sge::image::create_texture(
						create_path(
							sge::parse::json::find_member_exn<sge::parse::json::string>(
								o.members,
								FCPPT_TEXT("texture")),
							FCPPT_TEXT("textures/props")),
						params.systems.renderer(),
						params.systems.image_loader(),
						sge::renderer::filter::trilinear,
						sge::renderer::resource_flags::none)
				}
			},
			model));

	// We pull all the settings from the json because we need them in
	// the loop that follows.

	height_map::flatness_range const flatness =	
		json::parse_vector<height_map::scalar,2,sge::parse::json::float_type>(
			sge::parse::json::find_member_exn<sge::parse::json::array>(
				o.members,
				FCPPT_TEXT("flatness_range")));

	physics::scalar const penetration_depth = 
		static_cast<physics::scalar>(
			sge::parse::json::find_member_exn<sge::parse::json::float_type>(
				o.members,
				FCPPT_TEXT("penetration_depth")));

	physics::vec2 const scale_range = 
		json::parse_vector<physics::scalar,2,sge::parse::json::float_type>(
			sge::parse::json::find_member_exn<sge::parse::json::array>(
				o.members,
				FCPPT_TEXT("scale_range")));

	physics::vec3 const rotation_axis = 
		json::parse_vector<physics::scalar,3,sge::parse::json::float_type>(
			sge::parse::json::find_member_exn<sge::parse::json::array>(
				o.members,
				FCPPT_TEXT("rotation_axis")));

	std::size_t const count = 
		static_cast<std::size_t>(
			sge::parse::json::find_member_exn<sge::parse::json::int_type>(
				o.members,
				FCPPT_TEXT("count")));

	// Beginning of the actual algorithm

	random_engine rng_engine(
		random_seed());

	std::uniform_real_distribution<physics::scalar> twopi_rng(
		static_cast<physics::scalar>(0),
		fcppt::math::twopi<physics::scalar>());

	std::uniform_real_distribution<physics::scalar> scale_rng(
		scale_range[0],
		scale_range[1]);

	for (std::size_t i = 0; i < count; ++i)
	{
		physics::scalar const 
			rotation_angle = 
				twopi_rng(
					rng_engine),
			scaling = 
				scale_rng(
					rng_engine);

		physics::vec3 const origin = 
			fcppt::math::vector::structure_cast<physics::vec3>(
				height_map::random_point(
					params.height_map,
					params.water_level,
					rng_engine,
					flatness))
			+
			physics::vec3(
				0,
				model->bounding_box().bottom() * scaling
					- penetration_depth * scaling,
				0);

		FCPPT_ASSERT(model);

		// We cannot take this bounding box as the bounding box for the
		// broadphase since the model is rotated and scaled. We need to take
		// both into account.
		graphics::box const aabb = 
			model->bounding_box();

		physics::scalar box_edge_length = 
			scaling * 
			math::box_radius(
				aabb);

		instances_.push_back(
			new instance(
				fcppt::math::matrix::translation(
					fcppt::math::vector::structure_cast<graphics::vec3>(
						origin)) *
				fcppt::math::matrix::rotation_axis(
					rotation_angle,
					rotation_axis) *
				math::uniform_scaling_matrix(
					scaling),
				params.broadphase_manager,
				physics::ghost_parameters(
					broadphase_,
					physics::box(
						origin - 
						physics::vec3(
							static_cast<physics::scalar>(box_edge_length),
							static_cast<physics::scalar>(box_edge_length),
							static_cast<physics::scalar>(box_edge_length)),
						physics::dim3(
							2*box_edge_length,
							2*box_edge_length,
							2*box_edge_length)))));

		scene_manager_.insert(
			backends_.back(),
			instances_.back());
	}
}
