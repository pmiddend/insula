#ifndef INSULA_HEIGHT_MAP_OBJECT_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_OBJECT_HPP_INCLUDED

#include "../graphics/camera_fwd.hpp"
#include "../graphics/vec2.hpp"
#include "../graphics/vec3.hpp"
#include "../graphics/scalar.hpp"
#include "../graphics/shader_to_console.hpp"
#include "../graphics/shader.hpp"
#include "render_mode.hpp"
#include "array.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/index_buffer_ptr.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <sge/image/file_ptr.hpp>
#include <sge/console/object_fwd.hpp>

namespace insula
{
namespace height_map
{
class object
{
public:
	explicit
	object(
		graphics::camera const &,
		sge::renderer::device_ptr,
		sge::console::object &,
		height_map::array const &,
		graphics::vec2 const &cell_sizes,
		graphics::scalar const height_scaling,
		graphics::vec3 const &sun_direction,
		graphics::scalar const ambient_light,
		graphics::scalar const texture_scaling,
		graphics::scalar const water_height,
		sge::image::file_ptr const &gradient_texture_image,
		sge::image::file_ptr const &lower_texture_image,
		sge::image::file_ptr const &upper_texture_image);
	
	object &operator=(
		object &) = delete;

	object(
		object const &) = delete;
	
	void
	render(
		render_mode::type);

	void
	regenerate(
		graphics::vec2 const &cell_sizes,
		graphics::scalar const height_scaling,
		array const &raw);
private:
	graphics::camera const &camera_;
	sge::renderer::device_ptr const renderer_;
	sge::renderer::vertex_buffer_ptr vb_;
	sge::renderer::index_buffer_ptr ib_;
	graphics::shader shader_;
	graphics::shader_to_console shader_console_;
	sge::renderer::texture_ptr lower_texture_,upper_texture_,gradient_texture_;

	void 
	regenerate_buffers(
		graphics::vec2 const &cell_sizes,
		graphics::scalar const height_scaling,
		array const &raw,
		array const &stretched,
		array const &gradient);
};
}
}

#endif
