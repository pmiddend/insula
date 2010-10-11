#ifndef INSULA_GRAPHICS_BOX_HPP_INCLUDED
#define INSULA_GRAPHICS_BOX_HPP_INCLUDED

#include "scalar.hpp"
#include <fcppt/math/box/box.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/dim/dim.hpp>

namespace insula
{
namespace graphics
{
typedef
fcppt::math::box::basic
<
	scalar,
	3
>
box;
}
}

#endif
