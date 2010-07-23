#ifndef INSULA_HEIGHT_MAP_OBJECT_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_OBJECT_HPP_INCLUDED

#include "array.hpp"
#include "scalar.hpp"
#include "vector2.hpp"
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/index_buffer_ptr.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <fcppt/noncopyable.hpp>

namespace insula
{
namespace height_map
{
class object
{
FCPPT_NONCOPYABLE(object)
public:
	explicit
	object(
		sge::renderer::device_ptr,
		array const &heights,
		array const &heights_stretched,
		array const &gradient,
		scalar height_scaling,
		vector2 const &cell_sizes);
	
	void
	render();
private:
	sge::renderer::device_ptr const renderer_;
	sge::renderer::vertex_buffer_ptr const vb_;
	sge::renderer::index_buffer_ptr const ib_;
	sge::renderer::scoped_vertex_buffer const scoped_vb_;
};
}
}

#endif
