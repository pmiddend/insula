#ifndef INSULA_SKYDOME_OBJECT_HPP_INCLUDED
#define INSULA_SKYDOME_OBJECT_HPP_INCLUDED

#include "size_type.hpp"
#include "../graphics/camera.hpp"
#include "../graphics/shader.hpp"
#include "../graphics/shader_to_console.hpp"
#include <sge/console/object_fwd.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/index_buffer_ptr.hpp>

namespace insula
{
namespace skydome
{
class object
{
public:
	object(
		object const &) = delete;
	object &operator=(
		object const &) = delete;

	explicit
	object(
		graphics::camera const &,
		sge::renderer::device_ptr,
		sge::console::object &,
		graphics::scalar angle_in_degrees,
		// von oben nach unten
		size_type iterations_latitude,
		size_type iterations_longitude);
	
	void
	render();

	void
	regenerate_buffer(
		graphics::scalar,
		size_type,
		size_type);
private:
	graphics::camera const &camera_;
	sge::renderer::device_ptr const renderer_;
	sge::renderer::vertex_buffer_ptr vb_;
	sge::renderer::index_buffer_ptr ib_;
	graphics::shader shader_;
	graphics::shader_to_console shader_to_console_;
};
}
}

#endif
