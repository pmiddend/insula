#ifndef INSULA_HEIGHT_MAP_VEC2_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_VEC2_HPP_INCLUDED

#include "scalar.hpp"
#include <fcppt/math/vector/vector.hpp>

namespace insula
{
namespace height_map
{
typedef
fcppt::math::vector::static_
<
	scalar,
	2
>::type
vec2;
}
}

#endif
