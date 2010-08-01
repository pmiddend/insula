#include "../graphics/camera.hpp"
#include "../graphics/vec3.hpp"
#include "mirror_camera.hpp"
#include "../graphics/scoped_camera.hpp"
#include "vf/vertex_view.hpp"
#include "vf/position.hpp"
#include "vf/packed_position.hpp"
#include "vf/format.hpp"
#include <sge/renderer/device.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/image/color/format.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/size_type.hpp>
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
#include <sge/image/color/rgba8.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/dim_type.hpp>
#include <sge/image/multi_loader.hpp>
#include <sge/image/loader.hpp>
#include <sge/image/view/make_const.hpp>
#include <sge/image/file.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/container/bitfield/bitfield.hpp>
#include "../media_path.hpp"
#include "object.hpp"

insula::water::object::object(
	sge::renderer::device_ptr const _renderer,
	graphics::camera &_camera,
	graphics::scalar const _water_height,
	sge::image::multi_loader &_image_loader,
	sge::console::object &_console,
	graphics::scalar const _dimension,
	sge::renderer::dim_type const &reflection_texture_size,
	render_mode::type const _render_mode)
:
	renderer_(
		_renderer),
	camera_(
		_camera),
	water_height_(
		_water_height),
	image_loader_(
		_image_loader),
	shader_(
		renderer_,
		media_path()/FCPPT_TEXT("water_vertex.glsl"),
		media_path()/FCPPT_TEXT("water_fragment.glsl")),
	shader_console_(
		FCPPT_TEXT("water"),
		shader_,
		_console)
{
	regenerate(
		_dimension,
		reflection_texture_size,
		_render_mode);
}


void
insula::water::object::render()
{
	sge::renderer::state::scoped scoped_state(
		renderer_,
		sge::renderer::state::list
		 	(sge::renderer::state::cull_mode::off)
		 	(sge::renderer::state::depth_func::less));

	sge::renderer::scoped_vertex_buffer const scoped_vb_(
		renderer_,
		vb_);

	sge::renderer::glsl::scoped_program scoped_shader_(
		renderer_,
		shader_.program());

	sge::renderer::scoped_texture scoped_tex(
		renderer_,
		target_texture_,
		0);

	shader_.set_uniform(
		FCPPT_TEXT("perspective"),
		camera_.perspective());

	shader_.set_uniform(
		FCPPT_TEXT("rotation"),
		camera_.rotation());

	shader_.set_uniform(
		FCPPT_TEXT("translation"),
		camera_.translation());

	renderer_->render(
		sge::renderer::first_vertex(
			0),
		sge::renderer::vertex_count(
			vb_->size()),
		sge::renderer::nonindexed_primitive_type::triangle);
}

void
insula::water::object::update_reflection(
	std::function<void ()> const &render_callback)
{
	if (render_mode_ == render_mode::no_reflection)
		return;

	sge::renderer::scoped_target const starget(
		renderer_,
		target_);

	sge::renderer::scoped_block const sblock(
		renderer_); 

	std::tuple<graphics::vec3,graphics::gizmo> const new_camera = 
		mirror_camera(
			camera_.position(),
			camera_.axes(),
			water_height_);

	graphics::scoped_camera cam(
		camera_,
		std::get<0>(new_camera),
		std::get<1>(new_camera));

	{
		sge::renderer::glsl::scoped_program scoped_shader_(
			renderer_,
			shader_.program());

			shader_.set_uniform(
			FCPPT_TEXT("rotation_mirror"),
			camera_.rotation());

		shader_.set_uniform(
			FCPPT_TEXT("translation_mirror"),
			camera_.translation());
	}

	render_callback();
}

void
insula::water::object::regenerate(
	graphics::scalar const dimension,
	sge::renderer::dim_type const &reflection_texture_size,
	render_mode::type const _render_mode)
{
	render_mode_ = 
		_render_mode;

	// We have to activate the shader here because we want to fill the
	// vertex buffer with "custom" attributes.
	sge::renderer::glsl::scoped_program scoped_shader_(
		renderer_,
		shader_.program());

	shader_.set_uniform(
		FCPPT_TEXT("texture"),
		0);

	shader_.set_uniform(
		FCPPT_TEXT("reflection"),
		render_mode::reflection ? 1 : 0);

	switch (render_mode_)
	{
		case render_mode::reflection:
			target_texture_ = 
				renderer_->create_texture(
					reflection_texture_size,
					sge::image::color::format::rgb8,
					sge::renderer::filter::linear,
					sge::renderer::resource_flags::readable);

			target_ = 
				renderer_->create_target(
					target_texture_);
		break;
		case render_mode::no_reflection:
			target_texture_.reset();
			target_.reset();
			break;
	}

	vb_ = 
		renderer_->create_vertex_buffer(
			sge::renderer::vf::dynamic::make_format<vf::format>(),
			static_cast<sge::renderer::size_type>(
				6),
			sge::renderer::resource_flags::none);

	sge::renderer::scoped_vertex_lock const vblock(
		vb_,
		sge::renderer::lock_mode::writeonly);

	vf::vertex_view const vertices(
		vblock.value());

	vf::vertex_view::iterator vb_it(
		vertices.begin());

	(vb_it++)->set<vf::position>(
		vf::packed_position(0,water_height_,0));
	(vb_it++)->set<vf::position>(
		vf::packed_position(10*dimension,water_height_,0));
	(vb_it++)->set<vf::position>(
		vf::packed_position(10*dimension,water_height_,10*dimension));

	(vb_it++)->set<vf::position>(
		vf::packed_position(10*dimension,water_height_,10*dimension));
	(vb_it++)->set<vf::position>(
		vf::packed_position(0,water_height_,10*dimension));
	(vb_it++)->set<vf::position>(
		vf::packed_position(0,water_height_,0));
}
