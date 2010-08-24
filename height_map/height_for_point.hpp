#ifndef INSULA_HEIGHT_MAP_HEIGHT_FOR_POINT_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_HEIGHT_FOR_POINT_HPP_INCLUDED

#include "scalar.hpp"
#include "array.hpp"
#include "vec2.hpp"

namespace insula
{
namespace height_map
{
scalar
height_for_point(
	array const &heights,
	scalar cell_size,
	vec2 const &point);
}
}

#endif
