#ifndef INSULA_GRAPHICS_VEC4_HPP_INCLUDED
#define INSULA_GRAPHICS_VEC4_HPP_INCLUDED

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
	4
>::type
vec4;
}
}

#endif
