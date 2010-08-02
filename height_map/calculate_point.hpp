#ifndef INSULA_HEIGHT_MAP_CALCULATE_POINT_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_CALCULATE_POINT_HPP_INCLUDED

#include "vf/packed_normal.hpp"
#include "array.hpp"
#include "scalar.hpp"
#include <type_traits>

namespace insula
{
namespace height_map
{
vf::packed_normal const
calculate_point(
	array const &heights,
	scalar const height_scaling,
	scalar const &cell_size,
	std::make_signed<array::size_type>::type rx,
	std::make_signed<array::size_type>::type ry);
}
}

#endif
