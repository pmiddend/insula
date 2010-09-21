#ifndef INSULA_MODEL_SCOPED_HPP_INCLUDED

#define INSULA_MODEL_SCOPED_HPP_INCLUDED

#include "object_fwd.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/cull_mode.hpp>

namespace insula
{
namespace model
{
class scoped
{
public:
	scoped(scoped const &) = delete;
	scoped &operator=(scoped const &) = delete;

	explicit
	scoped(
		sge::renderer::device_ptr,
		object &,
		sge::renderer::state::cull_mode::type = sge::renderer::state::cull_mode::front);

	~scoped();
private:
	sge::renderer::device_ptr const renderer_;
	sge::renderer::scoped_vertex_buffer scoped_vb_;
	sge::renderer::state::scoped scoped_state_;
};
}
}

#endif
