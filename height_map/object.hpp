#ifndef INSULA_HEIGHT_MAP_OBJECT_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_OBJECT_HPP_INCLUDED

#include "../graphics/camera/object_fwd.hpp"
#include "../graphics/vec2.hpp"
#include "../graphics/vec3.hpp"
#include "../graphics/scalar.hpp"
#include "../graphics/shader.hpp"
#include "../graphics/box.hpp"
#include "vec2.hpp"
#include "render_mode.hpp"
#include "array.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/index_buffer_ptr.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <sge/renderer/state/cull_mode.hpp>
#include <sge/image/file_ptr.hpp>
#include <sge/console/object_fwd.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/math/box/basic_impl.hpp>

namespace insula
{
namespace height_map
{
/*
	This class represents a height field. It's also responsible for
	_drawing_ the height field. So I holds a vertex and an index
	pointer. You can customize the cell size and the height scaling and
	you can also pass some light parameters.

	It also blends the textures you pass it, currently that's hard-coded
	for two height textures and a gradient texture.

	This class might be split in the future, since it's pretty fat.

	The cell layout is:

	 ---
	|\  |
	| \ |
	|  \|
	 ---

	The height field's lower left corner is supposed to be at the
	origin, the class is mostly (!) suited for non-quadratic height
	fields.
 */
class object
{
public:
	explicit
	object(
		graphics::camera::object const &,
		sge::renderer::device_ptr,
		height_map::array const &,
		graphics::scalar const &cell_size,
		graphics::scalar const height_scaling,
		graphics::vec3 const &sun_direction,
		graphics::scalar const ambient_light,
		graphics::scalar const texture_scaling,
		sge::image::file_ptr const &gradient_texture_image,
		sge::image::file_ptr const &lower_texture_image,
		sge::image::file_ptr const &upper_texture_image);
	
	object &operator=(
		object &) = delete;

	object(
		object const &) = delete;
	
	void
	render(
		sge::renderer::state::cull_mode::type,
		fcppt::optional<graphics::scalar> const &clip_height = 
			fcppt::optional<graphics::scalar>());

	// The console_proxy needs this
	graphics::shader &
	shader();

	graphics::box const
	extents() const;

	array const &
	heights() const;
	
	graphics::scalar
	cell_size() const;

	graphics::scalar
	height_scaling() const;

	~object();
private:
	graphics::camera::object const &camera_;
	sge::renderer::device_ptr const renderer_;
	sge::renderer::vertex_buffer_ptr vb_;
	sge::renderer::index_buffer_ptr ib_;
	graphics::shader shader_;
	sge::renderer::texture_ptr lower_texture_,upper_texture_,gradient_texture_;
	graphics::box extents_;
	array heights_;
	graphics::scalar cell_size_;
	graphics::scalar height_scaling_;

	void
	regenerate(
		graphics::scalar const &cell_size,
		graphics::scalar const height_scaling,
		array const &raw);

	void 
	regenerate_buffers(
		graphics::scalar const &cell_size,
		graphics::scalar const height_scaling,
		array const &raw,
		array const &stretched,
		array const &gradient);
};
}
}

#endif
