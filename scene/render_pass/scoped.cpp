#include "scoped.hpp"
#include "object.hpp"
#include <sge/renderer/device.hpp>

insula::scene::render_pass::scoped::scoped(
	sge::renderer::device_ptr const renderer,
	graphics::camera::object &cam,
	object const &o)
:
	camera_(
		cam,
		o.camera()),
	target_(
		renderer,
		o.target()),
	block_(
		renderer)
{
	renderer->viewport(
		o.viewport());
}
