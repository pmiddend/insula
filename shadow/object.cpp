#include "object.hpp"
#include "parameters.hpp"
#include "../json/find_member.hpp"
#include "../scene/manager.hpp"
#include "../scene/render_pass/object.hpp"
#include "../gizmo/rotation_to_mat4.hpp"
#include "../math/sphere_point.hpp"
#include "../graphics/vec3.hpp"
#include <sge/systems/instance.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/renderer/pixel_pos.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/filter/point.hpp>
#include <sge/renderer/filter/trilinear.hpp>
#include <sge/renderer/no_depth_stencil_texture.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/dim_type.hpp>
#include <sge/renderer/default_target.hpp>
#include <sge/image/color/format.hpp>
// sge doesn't use the big headers
#include <fcppt/io/cout.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/math/pi.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/text.hpp>
#include <functional>

insula::shadow::object::object(
	parameters const &params)
:
	target_texture_(
		params.systems.renderer()->create_texture(
			json::find_member<sge::renderer::dim_type>(
				params.config_file,
				FCPPT_TEXT("shadow-map-size")),
			sge::image::color::format::rgb32f,
			sge::renderer::filter::trilinear,
			sge::renderer::resource_flags::none)),
	target_(
		params.systems.renderer()->create_target(
			target_texture_,
			sge::renderer::no_depth_stencil_texture())),
	sun_distance_(
		json::find_member<graphics::scalar>(
			params.config_file,
			FCPPT_TEXT("sun-distance"))),
	// Angle 0 would mean the sun is at the zenith. I want it to start at y=0
	sun_angle_(
		-fcppt::math::pi<graphics::scalar>()/2),
	base_position_(
		graphics::vec3::null())
{
	params.scene_manager.add(
		scene::render_pass::object(
			FCPPT_TEXT("shadow"),
			std::bind(
				&object::gizmo,
				this),
			[&target_texture_]() 
			{
				return 
					sge::renderer::viewport(
						sge::renderer::pixel_pos::null(),
						fcppt::math::dim::structure_cast<sge::renderer::screen_size>(
							target_texture_->dim())); 
			},
			[&target_]()
			{
				return target_;
			}),
		// Dependencies
		{});
}

sge::renderer::texture_ptr const
insula::shadow::object::texture()
{
	return target_texture_;
}

void
insula::shadow::object::update(
	time_delta const delta)
{
	sun_angle_ += delta / 2;
	if (sun_angle_ >= fcppt::math::pi<graphics::scalar>()/2)
		sun_angle_ = -fcppt::math::pi<graphics::scalar>()/2;
	update_signal_(
		sun_angle_,
		gizmo());
}

/*
insula::graphics::mat4 const
insula::shadow::object::mvp(
	graphics::mat4 const &projection) const
{
	return 
		projection 
			gizmo::to_mat4(gizmo())
			* gizmo::rotation_to_mat4(gizmo()) 
			* fcppt::math::matrix::translation(-gizmo().position());
}
*/

insula::graphics::gizmo const
insula::shadow::object::gizmo() const
{
	graphics::vec3 const 
		position = 
			math::sphere_point(
				sun_distance_,
				sun_angle_,
				static_cast<graphics::scalar>(0)),
		forward = 
			-fcppt::math::vector::normalize(
				position),
		right = 
			graphics::vec3(0,0,-1),
		up = 
			fcppt::math::vector::cross(
				forward,
				graphics::vec3(
					0,0,-1));

	return 
		graphics::gizmo(
			graphics::gizmo::init()
				.position(base_position_ - position)
				.forward(forward)
				.up(up)
				.right(right));
}

void
insula::shadow::object::base_position(
	graphics::vec3 const &v)
{
	base_position_ = v;
}

fcppt::signal::auto_connection
insula::shadow::object::register_callback(
	update_callback const &c)
{
	return 
		update_signal_.connect(
			c);
}
