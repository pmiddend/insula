#ifndef INSULA_MODEL_SCOPED_HPP_INCLUDED
#define INSULA_MODEL_SCOPED_HPP_INCLUDED

#include "object_fwd.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>

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
		object &);

	~scoped();
private:
	sge::renderer::device_ptr const renderer_;
	sge::renderer::vertex_buffer_ptr const vb_;
};
}
}

#endif
