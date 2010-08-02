#ifndef INSULA_HEIGHT_MAP_CALCULATE_NORMAL_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_CALCULATE_NORMAL_HPP_INCLUDED

#include "vf/packed_normal.hpp"
#include "array.hpp"
#include "scalar.hpp"

namespace insula
{
namespace height_map
{
vf::packed_normal const
calculate_normal(
	array const &heights,
	scalar const height_scaling,
	scalar const &cell_size,
	array::size_type const x,
	array::size_type const y);
}
}

#endif
