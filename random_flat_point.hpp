#ifndef INSULA_RANDOM_FLAT_POINT_HPP_INCLUDED
#define INSULA_RANDOM_FLAT_POINT_HPP_INCLUDED

#include "graphics/vec2.hpp"
#include "graphics/scalar.hpp"
#include "height_map/object_fwd.hpp"
#include "random_engine.hpp"
#include <fcppt/math/vector/basic_impl.hpp>

namespace insula
{
// Generates a random point on flat ground which is not in the
// water. Technically, this should get two random engines, one for
// each dimension
graphics::vec2 const
random_flat_point(
	height_map::object const &,
	graphics::scalar water_level,
	random_engine &engine);
}

#endif
