#ifndef INSULA_HEIGHT_MAP_RECT_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_RECT_HPP_INCLUDED

#include "scalar.hpp"
#include <fcppt/math/box/basic_decl.hpp>

namespace insula
{
namespace height_map
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
