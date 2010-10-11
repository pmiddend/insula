#ifndef INSULA_GRAPHICS_RECT_HPP_INCLUDED
#define INSULA_GRAPHICS_RECT_HPP_INCLUDED

#include "scalar.hpp"
#include <fcppt/math/box/box.hpp>

namespace insula
{
namespace graphics
{
typedef
fcppt::math::box::basic
<
	scalar,
	2
>
rect;
}
}

#endif
