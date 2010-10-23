#ifndef INSULA_SKYDOME_OBJECT_HPP_INCLUDED
#define INSULA_SKYDOME_OBJECT_HPP_INCLUDED

#include "size_type.hpp"
#include "parameters_fwd.hpp"
#include "../graphics/camera/object.hpp"
#include "../graphics/shader/object.hpp"
#include "../graphics/mat4.hpp"
#include "../graphics/scalar.hpp"
#include "../graphics/gizmo.hpp"
#include "../scene/function_backend.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/index_buffer_ptr.hpp>
#include <fcppt/signal/scoped_connection.hpp>

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
		parameters const &);

	~object();
private:
	graphics::camera::object const &camera_;
	sge::renderer::device_ptr const renderer_;
	sge::renderer::vertex_buffer_ptr vb_;
	sge::renderer::index_buffer_ptr ib_;
	graphics::shader::object shader_;
	graphics::mat4 const perspective_;
	scene::function_backend backend_;
	fcppt::signal::scoped_connection render_connection_;
	fcppt::signal::scoped_connection shadow_update_connection_;

	void
	render(
		scene::render_pass::object const &);

	void
	shadow_update(
		graphics::scalar sun_angle,
		graphics::gizmo const &);
};
}
}

#endif
