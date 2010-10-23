#include "vf/format.hpp"
#include "vf/vertex_view.hpp"
#include "object.hpp"
#include "parameters.hpp"
#include "../scene/manager.hpp"
#include "../graphics/camera/object.hpp"
#include "../graphics/camera/scoped.hpp"
#include "../graphics/shader/scoped.hpp"
#include "../graphics/shader/object.hpp"
#include "../graphics/shader/vf_to_string.hpp"
#include "../graphics/shader/variable.hpp"
#include "../graphics/shader/variable_type.hpp"
#include "../graphics/shader/sampler.hpp"
#include "../graphics/mat4.hpp"
#include "../media_path.hpp"
#include <sge/systems/instance.hpp>
#include <sge/renderer/device.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/filter/point.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/dim_type.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/default_target.hpp>
#include <sge/renderer/no_depth_stencil_texture.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/state/cull_mode.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/nonindexed_primitive_type.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/matrix/orthogonal_xy.hpp>
#include <fcppt/text.hpp>
#include <fcppt/io/cout.hpp>
#include <functional>
#include <cmath>

insula::overlay::object::object(
	parameters const &params)
:
	renderer_(
		params.systems.renderer()),
	camera_(
		params.camera),
	texture_backend_(
		params.scene_manager,
		{"overlay"},
		// Has to be high priority so it's drawn before the overlay
		// (otherwise it would override everything that's there)
		scene::backend_priority::high),
	user_backend_(
		params.scene_manager,
		{"overlay"}),
	overlay_texture_(
		renderer_->create_texture(
			fcppt::math::dim::structure_cast<sge::renderer::dim_type>(
				renderer_->screen_size()),
			sge::image::color::format::rgb8,
			sge::renderer::filter::point,
			sge::renderer::resource_flags::none)),
	target_(
		renderer_->create_target(
			overlay_texture_,
			sge::renderer::no_depth_stencil_texture())),
	vb_(
		renderer_->create_vertex_buffer(
			sge::renderer::vf::dynamic::make_format<vf::format>(),
			static_cast<sge::renderer::size_type>(
				6),
			sge::renderer::resource_flags::none)),
	quad_shader_(
		renderer_,
		media_path()/FCPPT_TEXT("quad_vertex.glsl"),
		media_path()/FCPPT_TEXT("quad_fragment.glsl"),
		graphics::shader::vf_to_string<vf::format>(),
		{
			graphics::shader::variable(
				"mvp",
				graphics::shader::variable_type::uniform,
				graphics::mat4()),
			graphics::shader::variable(
				"screen_size",
				graphics::shader::variable_type::const_,
				fcppt::math::dim::structure_cast<graphics::vec2>(
					renderer_->screen_size()))
		},
		{
			graphics::shader::sampler(
				"texture",
				overlay_texture_)
		}),
	internal_connection_(
		texture_backend_.register_callback(
			std::bind(
				&object::render,
				this,
				std::placeholders::_1)))
{
	params.scene_manager.add(
		scene::render_pass::object(
			FCPPT_TEXT("normal"),
			[&camera_]() 
			{ 
				return camera_.gizmo();
			},
			[this]() 
			{ 
				return 
					sge::renderer::viewport(
						sge::renderer::pixel_pos::null(),
						renderer_->screen_size()); 
			},
			[&target_]() 
			{ 
				return target_;
			}),
			{"water","shadow"});

	params.scene_manager.add(
		scene::render_pass::object(
			FCPPT_TEXT("overlay"),
			[&params]() 
			{ 
				return 
					graphics::gizmo(
						graphics::gizmo::init()
							.position(graphics::vec3(0,0,0))
							.forward(graphics::vec3(0,0,1))
							.up(graphics::vec3(0,1,0))
							.right(graphics::vec3(1,0,0))); 
			},
			[this]() 
			{ 
				return 
					sge::renderer::viewport(
						sge::renderer::pixel_pos::null(),
						renderer_->screen_size());
			},
			[]()
			{
				return sge::renderer::default_target();
			}),
		{"normal"});

	// We have to activate the shader here because we want to fill the
	// vertex buffer with "custom" attributes.
	sge::renderer::glsl::scoped_program scoped_shader_(
		renderer_,
		quad_shader_.program());

	{
		graphics::camera::scoped cam(
			params.camera,
			graphics::gizmo(
				graphics::gizmo::init()
					.position(graphics::vec3(0,0,-params.camera.near()))
					.forward(graphics::vec3(0,0,1))
					.up(graphics::vec3(0,1,0))
					.right(graphics::vec3(1,0,0))));
		quad_shader_.set_uniform(
			FCPPT_TEXT("mvp"),
			fcppt::math::matrix::orthogonal_xy<graphics::scalar>() 
				* params.camera.world());
	}


	sge::renderer::scoped_vertex_lock const vblock(
		vb_,
		sge::renderer::lock_mode::writeonly);

	vf::vertex_view const vertices(
		vblock.value());

	vf::vertex_view::iterator vb_it(
		vertices.begin());

	// Left top
	(vb_it++)->set<vf::position>(
		vf::position::packed_type(
			-1,
			1,
			0));

	// Left bottom
	(vb_it++)->set<vf::position>(
		vf::position::packed_type(
			-1,-1,0));

	// Right top
	(vb_it++)->set<vf::position>(
		vf::position::packed_type(
			1,1,0));

	// Right top
	(vb_it++)->set<vf::position>(
		vf::position::packed_type(
			1,1,0));

	// Left bottom
	(vb_it++)->set<vf::position>(
		vf::position::packed_type(
			-1,-1,0));

	// Right bottom
	(vb_it++)->set<vf::position>(
		vf::position::packed_type(
			1,-1,0));
}

sge::renderer::target_ptr const
insula::overlay::object::target()
{
	return target_;
}

fcppt::signal::auto_connection
insula::overlay::object::register_callback(
	scene::function_backend::callback const &c)
{
	return user_backend_.register_callback(c);
}

void
insula::overlay::object::render(
	scene::render_pass::object const &)
{
	sge::renderer::state::scoped scoped_state(
		renderer_,
		sge::renderer::state::list
			// We don't care about culling
		 	(sge::renderer::state::cull_mode::off)
			// ...and depth stuff, too
		 	(sge::renderer::state::depth_func::off));

	graphics::shader::scoped scoped_shader_(
		quad_shader_);

	sge::renderer::scoped_vertex_buffer const scoped_vb_(
		renderer_,
		vb_);

	renderer_->render(
		sge::renderer::first_vertex(
			0),
		sge::renderer::vertex_count(
			vb_->size()),
		sge::renderer::nonindexed_primitive_type::triangle);
}
