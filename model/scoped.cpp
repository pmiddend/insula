#include "scoped.hpp"
#include "object.hpp"
#include <sge/renderer/device.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/state/cull_mode.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/depth_func.hpp>

insula::model::scoped::scoped(
	sge::renderer::device_ptr const _renderer,
	object &o)
:
	renderer_(
		_renderer),
	scoped_vb_(
		renderer_,
		o.vb()),
	scoped_state_(
		renderer_,
		sge::renderer::state::list
			(sge::renderer::state::cull_mode::off)
			(sge::renderer::state::depth_func::less))
{
}

insula::model::scoped::~scoped()
{
}
