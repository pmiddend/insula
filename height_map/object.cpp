#include "object.hpp"
#include "../graphics/scalar.hpp"
#include "vf/format.hpp"
#include "vf/position.hpp"
#include "vf/texture_coordinate.hpp"
#include "vf/vertex_view.hpp"
#include "vf/packed_position.hpp"
#include "vf/packed_texture_coordinate.hpp"
#include "calculate_index_cell.hpp"
#include <sge/renderer/device.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/index/dynamic/format.hpp>
#include <sge/renderer/index/dynamic/view.hpp>
#include <sge/renderer/index/i16.hpp>
#include <sge/renderer/index/i32.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/scoped_index_lock.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/primitive_count.hpp>
#include <sge/renderer/index_buffer.hpp>
#include <sge/image/color/any/convert.hpp>
#include <sge/image/colors.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert_message.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/variant/apply_unary.hpp>
#include <boost/foreach.hpp>
#include <limits>

namespace
{
struct index_visitor
{
public:
	typedef 
	void 
	result_type;

	typedef 
	insula::height_map::array::size_type
	size_type;

	index_visitor(
		size_type const _w,
		size_type const _h)
	:
		w(
			_w),
		h(
			_h)
	{
	}

	template<typename T>
	result_type
	operator()(
		T const &t) const
	{
		typedef typename 
		T::value_type 
		value_type;

		typename T::iterator it = t.begin();

		for (size_type y = 0; y < static_cast<size_type>(h-1); ++y)
			for (size_type x = 0; x < static_cast<size_type>(w-1); ++x)
				BOOST_FOREACH(
					value_type const i,
					insula::height_map::calculate_index_cell<value_type>(
						static_cast<value_type>(
							x),
						static_cast<value_type>(
							y),
						static_cast<value_type>(
							w),
						static_cast<value_type>(
							h)))
					*it++ = i;
	}
private:
	size_type w,h;
};
}

insula::height_map::object::object(
	sge::renderer::device_ptr const _renderer,
	array const &heights,
	array const &heights_stretched,
	array const &grad,
	scalar const height_scaling,
	vector2 const &cell_sizes)
:
	renderer_(
		_renderer),
	vb_(
		renderer_->create_vertex_buffer(
			sge::renderer::vf::dynamic::make_format<vf::format>(),
			static_cast<sge::renderer::size_type>(
				heights.num_elements()),
			sge::renderer::resource_flags::none)),
	ib_(
		renderer_->create_index_buffer(
			sge::renderer::index::dynamic::format::i32,
			// q = (n-1)*(m-1) Quads
			// 2*q Tris
			// 3*2*q Indices
			static_cast<sge::renderer::size_type>(
				3*2*((heights.shape()[0]-1)*(heights.shape()[1]-1))),
			sge::renderer::resource_flags::none))
{
	FCPPT_ASSERT_MESSAGE(
		vb_->size() < 
		static_cast<sge::renderer::size_type>(
			std::numeric_limits<sge::renderer::index::i32>::max()),
		FCPPT_TEXT("The heightmap is too big for a 16 bit index!"));
	
	sge::renderer::scoped_vertex_lock const vblock(
		vb_,
		sge::renderer::lock_mode::writeonly);

	vf::vertex_view const vertices(
		vblock.value());

	vf::vertex_view::iterator vb_it(
		vertices.begin());

	for (array::size_type y = 0; y < heights.shape()[0]; ++y)
	{
		for (array::size_type x = 0; x < heights.shape()[1]; ++x)
		{
			vf::packed_position p(
				static_cast<graphics::scalar>(
					static_cast<scalar>(
						x) * 
					cell_sizes.x()),
				static_cast<graphics::scalar>(
					static_cast<scalar>(
						height_scaling) * 
					heights[y][x]),
				static_cast<graphics::scalar>(
					static_cast<scalar>(
						y) * 
					cell_sizes.y()));
			// Just to be safe, we put the 0 here
			(*vb_it).set<vf::position>(
				p);

			(*vb_it).set<vf::texture_coordinate>(
				vf::packed_texture_coordinate(
					heights_stretched[y][x],
					grad[y][x]));
			
			vb_it++;
		}
	}
	
	fcppt::variant::apply_unary(
		index_visitor(
			heights.shape()[0],
			heights.shape()[1]),
		sge::renderer::scoped_index_lock(
			ib_,
			sge::renderer::lock_mode::writeonly).value().any());
}

void
insula::height_map::object::render()
{
	sge::renderer::scoped_vertex_buffer const vb_context(
		renderer_,
		vb_);
	
	renderer_->render(
		ib_,
		sge::renderer::first_vertex(
			0),
		sge::renderer::vertex_count(
			vb_->size()),
		sge::renderer::indexed_primitive_type::triangle,
		sge::renderer::primitive_count(
			ib_->size() / 3),
		sge::renderer::first_index(
			0));
}
