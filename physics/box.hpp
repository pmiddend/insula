#ifndef INSULA_PHYSICS_BOX_HPP_INCLUDED
#define INSULA_PHYSICS_BOX_HPP_INCLUDED

#include "scalar.hpp"
#include <fcppt/math/box/box.hpp>

namespace insula
{
namespace physics
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
