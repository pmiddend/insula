#ifndef INSULA_HEIGHT_MAP_VEC3_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_VEC3_HPP_INCLUDED

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
	3
>::type
vec3;
}
}

#endif
