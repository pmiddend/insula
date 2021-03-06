#include "object.hpp"
#include "parameters.hpp"
#include "vf/vertex_view.hpp"
#include "vf/position.hpp"
#include "vf/texture_coordinate.hpp"
#include "vf/packed_position.hpp"
#include "vf/packed_texture_coordinate.hpp"
#include "vf/format.hpp"
#include "../graphics/camera/object.hpp"
#include "../graphics/vec3.hpp"
#include "../graphics/vec2.hpp"
#include "../graphics/dim2.hpp"
#include "../graphics/rect.hpp"
#include "../graphics/camera/scoped.hpp"
#include "../graphics/shader/vf_to_string.hpp"
#include "../graphics/shader/scoped.hpp"
#include "../gizmo/mirror_at_plane.hpp"
#include "../scene/render_pass/object.hpp"
#include "../scene/manager.hpp"
#include "../json/find_member.hpp"
#include "../media_path.hpp"
#include "../create_path.hpp"
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
#include <sge/image/create_texture.hpp>
#include <sge/image/file.hpp>
#include <sge/time/second.hpp>
#include <sge/systems/instance.hpp>
#include <fcppt/container/bitfield/bitfield.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/box/stretch.hpp>
#include <initializer_list>

insula::water::object::object(
	parameters const &params)
:
	scene::backend(
		params.scene_manager,
		{"normal"}),
	renderer_(
		params.systems.renderer()),
	target_texture_(
		renderer_->create_texture(
			json::find_member<sge::renderer::dim_type>(
				params.config_file,
				FCPPT_TEXT("water/reflection-size")),
			sge::image::color::format::rgb8,
			sge::renderer::filter::linear,
			sge::renderer::resource_flags::readable)),
	target_(
		renderer_->create_target(
			target_texture_,
			sge::renderer::no_depth_stencil_texture())),
	camera_(
		params.camera),
	water_level_(
		json::find_member<graphics::scalar>(
				params.config_file,
				FCPPT_TEXT("water/level"))),
	shader_(
		renderer_,
		media_path()/FCPPT_TEXT("water_vertex.glsl"),
		media_path()/FCPPT_TEXT("water_fragment.glsl"),
		graphics::shader::vf_to_string<vf::format>(),
		{
			graphics::shader::variable(
				"wave_height",
				graphics::shader::variable_type::const_,
				json::find_member<graphics::scalar>(
					params.config_file,
					FCPPT_TEXT("water/wave-height"))),
			graphics::shader::variable(
				"wind_speed",
				graphics::shader::variable_type::const_,
				json::find_member<graphics::scalar>(
					params.config_file,
					FCPPT_TEXT("water/wind-speed"))),
			graphics::shader::variable(
				"mvp",
				graphics::shader::variable_type::uniform,
				graphics::mat4()),
			graphics::shader::variable(
				"mvp_mirror",
				graphics::shader::variable_type::uniform,
				graphics::mat4()),
			graphics::shader::variable(
				"time",
				graphics::shader::variable_type::uniform,
				graphics::scalar())
		},
		{
			graphics::shader::sampler(
				"bump_texture",
				sge::image::create_texture(
					create_path(
						json::find_member<fcppt::string>(
							params.config_file,
							FCPPT_TEXT("water/bump-texture")),
						FCPPT_TEXT("textures")),
					renderer_,
					params.systems.image_loader(),
					sge::renderer::filter::linear,
					sge::renderer::resource_flags::none)),
			graphics::shader::sampler(
				"reflection_texture",
				target_texture_)
		}),
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
	graphics::box const stretched = 
		fcppt::math::box::stretch(
			params.extents,
			json::find_member<graphics::scalar>(
				params.config_file,
				FCPPT_TEXT("water/stretch-factor")));

	graphics::rect const extents(
		graphics::vec2(
			stretched.pos()[0],
			stretched.pos()[2]),
		graphics::dim2(
			stretched.dimension()[0],
			stretched.dimension()[2]));

	graphics::scalar const texture_scaling = 
		json::find_member<graphics::scalar>(
			params.config_file,
			FCPPT_TEXT("water/texture-scaling"));
	
	// We have to activate the shader here because we want to fill the
	// vertex buffer with "custom" attributes.
	sge::renderer::glsl::scoped_program scoped_shader_(
		renderer_,
		shader_.program());

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
			texture_scaling,
			0)); 

	(vb_it)->set<vf::position>(
		vf::packed_position(
			extents.pos().x() + extents.dimension().w(),
			water_level_,
			extents.pos().y() + extents.dimension().h()));
	(vb_it++)->set<vf::texture_coordinate>(
		vf::packed_texture_coordinate(
			texture_scaling,
			texture_scaling)); 

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

	params.scene_manager.add(
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
