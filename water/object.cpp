#include "../graphics/camera.hpp"
#include "../graphics/vec3.hpp"
#include "mirror_camera.hpp"
#include "../graphics/scoped_camera.hpp"
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
#include <sge/image/file.hpp>
#include <sge/time/second.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/container/bitfield/bitfield.hpp>
#include "../media_path.hpp"
#include "object.hpp"

insula::water::object::object(
	sge::renderer::device_ptr const _renderer,
	graphics::camera &_camera,
	graphics::scalar const _water_level,
	graphics::rect const &extents,
	sge::renderer::dim_type const &reflection_texture_size,
	sge::image::file_ptr const _bump_texture,
	graphics::scalar const texture_scaling)
:
	renderer_(
		_renderer),
	bump_texture_(
		renderer_->create_texture(
			_bump_texture->view(),
			sge::renderer::filter::linear,
			sge::renderer::resource_flags::none)),
	camera_(
		_camera),
	water_level_(
		_water_level),
	shader_(
		renderer_,
		media_path()/FCPPT_TEXT("water_vertex.glsl"),
		media_path()/FCPPT_TEXT("water_fragment.glsl")),
	wave_timer_(
		sge::time::second(1)),
	current_time_(
		static_cast<graphics::scalar>(0))
{
	regenerate(
		extents,
		reflection_texture_size,
		texture_scaling);
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

	sge::renderer::scoped_texture scoped_tex0(
		renderer_,
		target_texture_,
		0);

	sge::renderer::scoped_texture scoped_tex1(
		renderer_,
		bump_texture_,
		1);

	shader_.set_uniform(
		FCPPT_TEXT("perspective"),
		camera_.perspective());

	shader_.set_uniform(
		FCPPT_TEXT("rotation"),
		camera_.rotation());

	shader_.set_uniform(
		FCPPT_TEXT("translation"),
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

insula::graphics::shader &
insula::water::object::shader()
{
	return shader_;
}

insula::graphics::scalar
insula::water::object::water_level() const
{
	return water_level_;
}

void
insula::water::object::update_reflection(
	std::function<void ()> const &render_callback)
{
	sge::renderer::scoped_target const starget(
		renderer_,
		target_);

	sge::renderer::scoped_block const sblock(
		renderer_); 

	std::tuple<graphics::vec3,graphics::gizmo> const new_camera = 
		mirror_camera(
			camera_.position(),
			camera_.axes(),
			water_level_);

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
	graphics::rect const &extents,
	sge::renderer::dim_type const &reflection_texture_size,
	graphics::scalar const texture_scaling)
{
	// We have to activate the shader here because we want to fill the
	// vertex buffer with "custom" attributes.
	sge::renderer::glsl::scoped_program scoped_shader_(
		renderer_,
		shader_.program());

	shader_.set_uniform(
		FCPPT_TEXT("reflection_texture"),
		0);

	shader_.set_uniform(
		FCPPT_TEXT("bump_texture"),
		1);

	target_texture_ = 
		renderer_->create_texture(
			reflection_texture_size,
			sge::image::color::format::rgb8,
			sge::renderer::filter::linear,
			sge::renderer::resource_flags::readable);

	target_ = 
		renderer_->create_target(
			target_texture_);

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
}
