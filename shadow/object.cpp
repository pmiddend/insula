#include "object.hpp"
#include "parameters.hpp"
#include "../scene/manager.hpp"
#include "../scene/render_pass/object.hpp"
#include "../gizmo/rotation_to_mat4.hpp"
#include <sge/systems/instance.hpp>
#include <sge/renderer/dim_type.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/renderer/pixel_pos.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/filter/point.hpp>
#include <sge/renderer/filter/trilinear.hpp>
#include <sge/renderer/no_depth_stencil_texture.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/default_target.hpp>
#include <sge/image/color/format.hpp>
#include <fcppt/math/matrix/translation.hpp>
// sge doesn't use the big headers
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/math/dim/output.hpp>

insula::shadow::object::object(
	parameters const &params)
:
	target_texture_(
		params.systems.renderer()->create_texture(
			fcppt::math::dim::structure_cast<sge::renderer::dim_type>(
				params.texture_size),
			sge::image::color::format::rgb32f,
			sge::renderer::filter::trilinear,
			sge::renderer::resource_flags::none)),
	target_(
		params.systems.renderer()->create_target(
			target_texture_,
			sge::renderer::no_depth_stencil_texture())),
	sun_gizmo_(
		params.sun_gizmo)
{
	fcppt::io::cout << "Texture size is " << params.texture_size << "\n";
	params.scene_manager.add(
		scene::render_pass::object(
			FCPPT_TEXT("shadow"),
			[&sun_gizmo_]() 
			{ 
				return sun_gizmo_; 
			},
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

insula::graphics::mat4 const
insula::shadow::object::mvp(
	graphics::mat4 const &projection) const
{
	return 
		projection 
			* gizmo::rotation_to_mat4(sun_gizmo_) 
			* fcppt::math::matrix::translation(sun_gizmo_.position() * -1.0f);
}

sge::renderer::texture_ptr const
insula::shadow::object::texture()
{
	return target_texture_;
}
