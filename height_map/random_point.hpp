#ifndef INSULA_HEIGHT_MAP_RANDOM_POINT_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_RANDOM_POINT_HPP_INCLUDED

#include "vec3.hpp"
#include "scalar.hpp"
#include "flatness_range.hpp"
#include "object_fwd.hpp"
#include "../random_engine.hpp"
#include <fcppt/math/vector/basic_impl.hpp>
#include <utility>

namespace insula
{
namespace height_map
{
// Generates a random point on the ground which is not in the
// water. Technically, this should get two random engines, one for
// each dimension
vec3 const
random_point(
	height_map::object const &,
	scalar water_level,
	random_engine &engine,
	flatness_range const &flatness_range);
}
}

#endif
