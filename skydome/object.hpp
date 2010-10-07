#ifndef INSULA_SKYDOME_OBJECT_HPP_INCLUDED
#define INSULA_SKYDOME_OBJECT_HPP_INCLUDED

#include "size_type.hpp"
#include "../graphics/camera/object.hpp"
#include "../graphics/shader/object.hpp"
#include "parameters_fwd.hpp"
#include "../scene/backend.hpp"
#include "parameters_fwd.hpp"
#include <fcppt/math/matrix/basic_impl.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/index_buffer_ptr.hpp>

namespace insula
{
namespace skydome
{
class object
:
	public scene::backend
{
public:
	object(
		object const &) = delete;
	object &operator=(
		object const &) = delete;

	explicit
	object(
		parameters const &);

	void
	begin(
		scene::render_pass::object const &);

	void 
	end(
		scene::render_pass::object const &);

	~object();
private:
	graphics::camera::object const &camera_;
	sge::renderer::device_ptr const renderer_;
	sge::renderer::vertex_buffer_ptr vb_;
	sge::renderer::index_buffer_ptr ib_;
	graphics::shader::object shader_;
	graphics::mat4 const perspective_;
};
}
}

#endif
