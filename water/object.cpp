#include "../graphics/camera/object.hpp"
#include "../graphics/vec3.hpp"
#include "../graphics/camera/scoped.hpp"
#include "../graphics/shader/vf_to_string.hpp"
#include "../graphics/shader/scoped.hpp"
#include "../gizmo/mirror_at_plane.hpp"
#include "../scene/render_pass/object.hpp"
#include "../scene/manager.hpp"
#include "vf/vertex_view.hpp"
#include "vf/position.hpp"
#include "vf/texture_coordinate.hpp"
#include "vf/packed_position.hpp"
#include "vf/packed_texture_coordinate.hpp"
#include "vf/format.hpp"
#include <sge/renderer/device.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/pixel_pos.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/resource_flags_field.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/scoped_texture_lock.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/scoped_texture.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/cull_mode.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/target.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/no_depth_stencil_texture.hpp>
#include <sge/image/color/rgba8.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/dim_type.hpp>
#include <sge/image/file.hpp>
#include <sge/time/second.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/container/bitfield/bitfield.hpp>
#include <fcppt/assign/make_container.hpp>
#include "../media_path.hpp"
#include "object.hpp"

insula::water::object::object(
	sge::renderer::device_ptr const _renderer,
	graphics::camera::object &_camera,
	graphics::scalar const _water_level,
	graphics::rect const &extents,
	sge::renderer::dim_type const &reflection_texture_size,
	sge::image::file_ptr const _bump_texture,
	graphics::scalar const texture_scaling,
	graphics::scalar const wave_height,
	graphics::scalar const wind_speed,
	scene::manager &scene_manager
)
:
	scene::backend(
		scene_manager,
		{"normal"}),
	renderer_(
		_renderer),
	target_texture_(
		renderer_->create_texture(
			reflection_texture_size,
			sge::image::color::format::rgb8,
			sge::renderer::filter::linear,
			sge::renderer::resource_flags::readable)),
	bump_texture_(
		),
	target_(
		renderer_->create_target(
			target_texture_,
			sge::renderer::no_depth_stencil_texture())),
	camera_(
		_camera),
	water_level_(
		_water_level),
	shader_(
		renderer_,
		media_path()/FCPPT_TEXT("water_vertex.glsl"),
		media_path()/FCPPT_TEXT("water_fragment.glsl"),
		graphics::shader::vf_to_string<vf::format>(),
		fcppt::assign::make_container<graphics::shader::variable_sequence>
			(
			graphics::shader::variable(
				"wave_height",
				graphics::shader::variable_type::const_,
				wave_height))
			(
			graphics::shader::variable(
				"wind_speed",
				graphics::shader::variable_type::const_,
				wind_speed))
			(
			graphics::shader::variable(
				"mvp",
				graphics::shader::variable_type::uniform,
				graphics::mat4()))
			(
			graphics::shader::variable(
				"mvp_mirror",
				graphics::shader::variable_type::uniform,
				graphics::mat4()))
			(
			graphics::shader::variable(
				"time",
				graphics::shader::variable_type::uniform,
				graphics::scalar())),
		fcppt::assign::make_container<graphics::shader::sampler_sequence>
			(
			graphics::shader::sampler(
				"bump_texture",
				renderer_->create_texture(
					_bump_texture->view(),
					sge::renderer::filter::linear,
					sge::renderer::resource_flags::none)))
			(
			graphics::shader::sampler(
				"reflection_texture"))),
	vb_(
		renderer_->create_vertex_buffer(
			sge::renderer::vf::dynamic::make_format<vf::format>(),
			static_cast<sge::renderer::size_type>(
				6),
			sge::renderer::resource_flags::none)),
	wave_timer_(
		sge::time::second(1)),
	current_time_(
		static_cast<graphics::scalar>(0))
{
	// We have to activate the shader here because we want to fill the
	// vertex buffer with "custom" attributes.
	sge::renderer::glsl::scoped_program scoped_shader_(
		renderer_,
		shader_.program());

	shader_.update_texture(
		"reflection_texture",
		target_texture_);

	sge::renderer::scoped_vertex_lock const vblock(
		vb_,
		sge::renderer::lock_mode::writeonly);

	vf::vertex_view const vertices(
		vblock.value());

	vf::vertex_view::iterator vb_it(
		vertices.begin());

	(vb_it)->set<vf::position>(
		vf::packed_position(
			extents.pos().x(),
			water_level_,
			extents.pos().y()));
	(vb_it++)->set<vf::texture_coordinate>(
		vf::packed_texture_coordinate(
			0,0)); 

	(vb_it)->set<vf::position>(
		vf::packed_position(
			extents.pos().x() + extents.dimension().w(),
			water_level_,
			extents.pos().y()));
	(vb_it++)->set<vf::texture_coordinate>(
		vf::packed_texture_coordinate(
			texture_scaling,0)); 

	(vb_it)->set<vf::position>(
		vf::packed_position(
			extents.pos().x() + extents.dimension().w(),
			water_level_,
			extents.pos().y() + extents.dimension().h()));
	(vb_it++)->set<vf::texture_coordinate>(
		vf::packed_texture_coordinate(
			texture_scaling,texture_scaling)); 

	(vb_it)->set<vf::position>(
		vf::packed_position(
			extents.pos().x() + extents.dimension().w(),
			water_level_,
			extents.pos().y() + extents.dimension().h()));
	(vb_it++)->set<vf::texture_coordinate>(
		vf::packed_texture_coordinate(
			texture_scaling,texture_scaling)); 

	(vb_it)->set<vf::position>(
		vf::packed_position(
			extents.pos().x(),
			water_level_,
			extents.pos().y() + extents.dimension().h()));
	(vb_it++)->set<vf::texture_coordinate>(
		vf::packed_texture_coordinate(
			0,texture_scaling)); 

	(vb_it)->set<vf::position>(
		vf::packed_position(
			extents.pos().x(),
			water_level_,
			extents.pos().y()));
	(vb_it)->set<vf::texture_coordinate>(
		vf::packed_texture_coordinate(
			0,0)); 

	scene_manager.add(
		scene::render_pass::object(
			FCPPT_TEXT("water"),
			[&camera_,&water_level_]()
			{
				return 
					gizmo::mirror_at_plane<graphics::scalar>(
						camera_.gizmo(),
						water_level_);
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


void
insula::water::object::begin(
	scene::render_pass::object const &)
{
	sge::renderer::state::scoped scoped_state(
		renderer_,
		sge::renderer::state::list
		 	(sge::renderer::state::cull_mode::off)
		 	(sge::renderer::state::depth_func::less));

	graphics::shader::scoped scoped_shader_(
		shader_);

	{
		graphics::camera::scoped cam(
			camera_,
			gizmo::mirror_at_plane<graphics::scalar>(
				camera_.gizmo(),
				water_level_));
			shader_.set_uniform(
				FCPPT_TEXT("mvp_mirror"),
				camera_.mvp());
	}

	sge::renderer::scoped_vertex_buffer const scoped_vb_(
		renderer_,
		vb_);

	shader_.set_uniform(
		FCPPT_TEXT("mvp"),
		camera_.perspective() * 
		camera_.rotation() * 
		camera_.translation());

	current_time_ += 
		wave_timer_.reset();

	shader_.set_uniform(
		FCPPT_TEXT("time"),
		current_time_);

	renderer_->render(
		sge::renderer::first_vertex(
			0),
		sge::renderer::vertex_count(
			vb_->size()),
		sge::renderer::nonindexed_primitive_type::triangle);
}

void
insula::water::object::end(
	scene::render_pass::object const &)
{
}

insula::graphics::scalar
insula::water::object::water_level() const
{
	return water_level_;
}

insula::water::object::~object() 
{}
