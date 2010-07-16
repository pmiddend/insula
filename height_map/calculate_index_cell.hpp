#ifndef INSULA_HEIGHT_MAP_CALCULATE_INDEX_CELL_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_CALCULATE_INDEX_CELL_HPP_INCLUDED

#include <fcppt/tr1/array.hpp>

namespace insula
{
namespace height_map
{
template<typename T>
std::tr1::array<T,6> const
calculate_index_cell(
	T const x,
	T const y,
	T const w,
	T const)
{
	std::tr1::array<T,6> a;
	T const 
		base = 
			static_cast<T>(
				y * w + x),
		nextbase = 
			static_cast<T>(
				(y+1) * w + x);

	a[0] = base;
	a[1] = static_cast<T>(nextbase+1);
	a[2] = static_cast<T>(base+1);
	a[3] = base;
	a[4] = nextbase;
	a[5] = static_cast<T>(nextbase+1);
	return a;
}
}
}

#endif
