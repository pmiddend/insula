#ifndef INSULA_GRAPHICS_VEC3_HPP_INCLUDED
#define INSULA_GRAPHICS_VEC3_HPP_INCLUDED

#include "scalar.hpp"
#include <fcppt/math/vector/static.hpp>

namespace insula
{
namespace graphics
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
