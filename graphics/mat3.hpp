#ifndef INSULA_GRAPHICS_MAT3_HPP_INCLUDED
#define INSULA_GRAPHICS_MAT3_HPP_INCLUDED

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
	3,
	3
>::type
mat3;
}
}

#endif
