#ifndef INSULA_HEIGHT_MAP_RANDOM_FLAT_POINT_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_RANDOM_FLAT_POINT_HPP_INCLUDED

#include "vec2.hpp"
#include "scalar.hpp"
#include "object_fwd.hpp"
#include "../random_engine.hpp"
#include <fcppt/math/vector/basic_impl.hpp>

namespace insula
{
namespace height_map
{
// Generates a random point on flat ground which is not in the
// water. Technically, this should get two random engines, one for
// each dimension
vec2 const
random_flat_point(
	height_map::object const &,
	scalar water_level,
	random_engine &engine);
}
}

#endif
