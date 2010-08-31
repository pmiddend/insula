#ifndef INSULA_SKYDOME_OBJECT_HPP_INCLUDED
#define INSULA_SKYDOME_OBJECT_HPP_INCLUDED

#include "size_type.hpp"
#include "gradient.hpp"
#include "../graphics/camera/object.hpp"
#include "../graphics/shader.hpp"
#include <fcppt/math/matrix/basic_impl.hpp>
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
		graphics::camera::object const &,
		sge::renderer::device_ptr,
		graphics::scalar angle_in_degrees,
		// von oben nach unten
		size_type iterations_latitude,
		size_type iterations_longitude,
		insula::skydome::gradient const &);
	
	void
	render();

	void
	regenerate_buffer(
		graphics::scalar,
		size_type,
		size_type);

	graphics::shader_old &
	shader();

	insula::skydome::gradient const
	gradient() const;

	~object();
private:
	graphics::camera::object const &camera_;
	sge::renderer::device_ptr const renderer_;
	sge::renderer::vertex_buffer_ptr vb_;
	sge::renderer::index_buffer_ptr ib_;
	graphics::shader_old shader_;
	graphics::mat4 const perspective_;
	insula::skydome::gradient gradient_;
};
}
}

#endif
