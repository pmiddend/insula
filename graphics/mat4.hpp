#ifndef INSULA_GRAPHICS_MAT4_HPP_INCLUDED
#define INSULA_GRAPHICS_MAT4_HPP_INCLUDED

#include "scalar.hpp"
#include <fcppt/math/matrix/static.hpp>

namespace insula
{
namespace graphics
{
typedef
fcppt::math::matrix::static_
<
	scalar,
	4,
	4
>::type
mat4;
}
}

#endif
