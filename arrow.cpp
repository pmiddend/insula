#include "arrow.hpp"
#include "create_path.hpp"
#include "timed_output.hpp"
#include "nugget/instance.hpp"
#include "arrow_parameters.hpp"
#include "graphics/shader/scoped.hpp"
#include "graphics/shader/object.hpp"
#include "graphics/camera/object.hpp"
#include "physics/vec2.hpp"
#include "physics/vec3.hpp"
#include "json/parse_vector.hpp"
#include "model/scoped.hpp"
#include <sge/systems/instance.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/string.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/model/loader.hpp>
#include <sge/image/create_texture.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/filter/trilinear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/matrix/rotation_y.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/dot.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/normalize.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/math/vector/angle_between.hpp>
#include <fcppt/math/vector/atan2.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/text.hpp>

insula::arrow::arrow(
	arrow_parameters const &params)
:
	model_(
		params.systems.md3_loader()->load(
			create_path(
				sge::parse::json::find_member_exn<sge::parse::json::string>(
					params.json.members,
					FCPPT_TEXT("model")),
				FCPPT_TEXT("models")),
			sge::model::load_flags::switch_yz),
		params.systems.renderer()),
	texture_(
		sge::image::create_texture(
			create_path(
				sge::parse::json::find_member_exn<sge::parse::json::string>(
					params.json.members,
					FCPPT_TEXT("texture")),
				FCPPT_TEXT("textures")),
			params.systems.renderer(),
			params.systems.image_loader(),
			sge::renderer::filter::trilinear,
			sge::renderer::resource_flags::none)),
	model_shader_(
		params.model_shader),
	renderer_(
		params.systems.renderer()),
	camera_(
		params.camera),
	nuggets_(
		params.nuggets),
	offset_(
		json::parse_vector<graphics::scalar,3,sge::parse::json::float_type>(
			sge::parse::json::find_member_exn<sge::parse::json::array>(
				params.json.members,
				FCPPT_TEXT("offset"))))
{
}

void
insula::arrow::render()
{
	model_shader_.update_texture(
		"texture",
		texture_);

	graphics::shader::scoped scoped_shader(
		model_shader_);

	model::scoped scoped_model(
		renderer_,
		model_);
	
	physics::vec2 const 
		forward(
			camera_.gizmo().forward().x(),
			camera_.gizmo().forward().z());

	physics::vec3 const diff = 
		nuggets_.closest_nugget(
			fcppt::math::vector::structure_cast<physics::vec3>(
				camera_.gizmo().position())) - 
		fcppt::math::vector::structure_cast<physics::vec3>(
			camera_.gizmo().position());

	physics::vec2 const
		to_nugget(
			diff.x(),
			diff.z());

	model_shader_.set_uniform(
		"mvp",
		camera_.perspective() * 

		fcppt::math::matrix::translation(
			offset_) * 

		// NOTE: Yes, this is relative to the camera, so not perfectly
		// alright, I'll have to fix that some day
		fcppt::math::matrix::rotation_y(

			// See http://www.euclideanspace.com/maths/geometry/trig/inverse/index.htm

			(*fcppt::math::vector::atan2(forward)) - (*fcppt::math::vector::atan2(to_nugget))

			/*
			std::acos(
				fcppt::math::vector::dot(
					to_nugget,
					forward)

				/

				(length(to_nugget)*length(forward)))
			*/

			/*
			 (*fcppt::math::vector::angle_between<graphics::scalar>(
				to_nugget,
				forward))*/

		)
		/*
		fcppt::math::matrix::rotation_y(
			static_cast<graphics::scalar>(
				std::acos(
					fcppt::math::vector::dot(
						fcppt::math::vector::structure_cast<physics::vec3>(
							camera_.gizmo().forward()),
						fcppt::math::vector::normalize(
							fcppt::math::vector::structure_cast<physics::vec3>(
								camera_.gizmo().position()) - 
						nuggets_.closest_nugget(
							fcppt::math::vector::structure_cast<physics::vec3>(
								camera_.gizmo().position())))))))*/);

	model_.render();
}

insula::arrow::~arrow() {}
