#include "scoped.hpp"
#include "object.hpp"
#include <sge/renderer/device.hpp>

insula::model::scoped::scoped(
	sge::renderer::device_ptr const _renderer,
	object &o)
:
	renderer_(
		_renderer),
	vb_(
		o.vb())
{
	renderer_->set_vertex_buffer(
		vb_);
}

insula::model::scoped::~scoped()
{
	renderer_->unset_vertex_buffer(
		vb_);
}
