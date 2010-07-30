#include "../graphics/camera.hpp"
#include "../graphics/vec3.hpp"
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
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/cull_mode.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <sge/renderer/state/color.hpp>
#include <sge/image/color/rgba8.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/image/multi_loader.hpp>
#include <sge/image/loader.hpp>
#include <sge/image/view/make_const.hpp>
#include <sge/image/file.hpp>
#include <mizuiro/color/init.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/cross.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/container/bitfield/bitfield.hpp>
#include "../media_path.hpp"
#include "object.hpp"

namespace
{
class scoped_camera
{
public:
	scoped_camera(scoped_camera const &) = delete;
	scoped_camera &operator=(scoped_camera const &) = delete;
	
	explicit
	scoped_camera(
		insula::graphics::camera &_camera,
		insula::graphics::vec3 const position_new,
		insula::graphics::gizmo const &gizmo_new)
	:
		camera_(
			_camera),
		// Save the old state...
		position_(
			camera_.position()),
		gizmo_(
			camera_.axes())
	{
		camera_.position(
			position_new);
		// ... and set the new state
		camera_.axes(
			gizmo_new);
	}

	~scoped_camera()
	{
		camera_.axes(
			gizmo_);
		camera_.position(
			position_);
	}
private:
	insula::graphics::camera &camera_;
	insula::graphics::vec3 const position_;
	insula::graphics::gizmo const gizmo_;
};
}

insula::water::object::object(
	sge::renderer::device_ptr const _renderer,
	graphics::camera &_camera,
	graphics::scalar const _water_height,
	sge::image::multi_loader &_image_loader,
	sge::console::object &_console,
	graphics::scalar const _dimension)
:
	renderer_(
		_renderer),
	target_(
		renderer_->create_texture(
			sge::renderer::texture::dim_type(
				fcppt::math::dim::structure_cast<sge::renderer::texture::dim_type>(
					renderer_->screen_size())),
				sge::image::color::format::rgb8,
				sge::renderer::filter::linear,
				sge::renderer::resource_flags::readable)),
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
		_dimension);
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
		target_,
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

	{
		graphics::vec3 const 
			new_position(
				camera_.position().x(),
				-camera_.position().y() + static_cast<graphics::scalar>(2)*water_height_,
				camera_.position().z()),
			target(
				camera_.position() + camera_.axes().forward()),
			new_target(
				graphics::vec3(
					target.x(),
					-target.y() + static_cast<graphics::scalar>(2)*water_height_,
					target.z())),
			new_forward(
				new_target - new_position),
			new_right(
				camera_.axes().right()),
			new_up(
				cross(
					new_forward,
					new_right));

		scoped_camera cam(
			camera_,
			new_position,
			graphics::gizmo_init()
				.forward(new_forward)
				.right(new_right)
				.up(new_up));

		shader_.set_uniform(
			FCPPT_TEXT("rotation_mirror"),
			camera_.rotation());

		shader_.set_uniform(
			FCPPT_TEXT("translation_mirror"),
			camera_.translation());
	}

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
	{
		sge::renderer::scoped_target const starget(
			renderer_,
			target_);

		sge::renderer::state::scoped const sstate(
			renderer_,
			sge::renderer::state::list
				(sge::renderer::state::bool_::clear_zbuffer = true)
				(sge::renderer::state::float_::zbuffer_clear_val = 1.f)
				(sge::renderer::state::bool_::clear_backbuffer = true)
				(sge::renderer::state::color::clear_color = 
					sge::image::color::rgba8(
						(mizuiro::color::init::red %= 0.765) 
						(mizuiro::color::init::green %= 0.87) 
						(mizuiro::color::init::blue %= 1.0) 
						(mizuiro::color::init::alpha %= 1.0))));

		sge::renderer::scoped_block const sblock(
			renderer_);

		graphics::vec3 const 
			new_position(
				camera_.position().x(),
				-camera_.position().y() + static_cast<graphics::scalar>(2)*water_height_,
				camera_.position().z()),
			target(
				camera_.position() + camera_.axes().forward()),
			new_target(
				graphics::vec3(
					target.x(),
					-target.y() + static_cast<graphics::scalar>(2)*water_height_,
					target.z())),
			new_forward(
				new_target - new_position),
			new_right(
				camera_.axes().right()),
			new_up(
				cross(
					new_forward,
					new_right));

		scoped_camera cam(
			camera_,
			new_position,
			graphics::gizmo_init()
				.forward(new_forward)
				.right(new_right)
				.up(new_up));

		render_callback();
	}
	
	/*
	// FIXME: Do we really need a lock here? We only want to read.
	sge::renderer::scoped_texture_lock texlock(
		target_,
		sge::renderer::lock_mode::readwrite);

	image_loader_.loaders().at(0)->create(
		sge::image::view::make_const(
			texlock.value()))->save(
		filename);
*/
}

void
insula::water::object::regenerate(
	graphics::scalar const dimension)
{
	// We have to activate the shader here because we want to fill the
	// vertex buffer with "custom" attributes.
	sge::renderer::glsl::scoped_program scoped_shader_(
		renderer_,
		shader_.program());

	shader_.set_uniform(
		FCPPT_TEXT("texture"),
		0);

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
		vf::packed_position(dimension,water_height_,0));
	(vb_it++)->set<vf::position>(
		vf::packed_position(dimension,water_height_,dimension));

	(vb_it++)->set<vf::position>(
		vf::packed_position(dimension,water_height_,dimension));
	(vb_it++)->set<vf::position>(
		vf::packed_position(0,water_height_,dimension));
	(vb_it++)->set<vf::position>(
		vf::packed_position(0,water_height_,0));
}
