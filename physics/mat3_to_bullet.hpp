#ifndef INSULA_PHYSICS_MAT3_TO_BULLET_HPP_INCLUDED
#define INSULA_PHYSICS_MAT3_TO_BULLET_HPP_INCLUDED

#include <LinearMath/btMatrix3x3.h>
#include "mat3.hpp"

namespace insula
{
namespace physics
{
btMatrix3x3 const
mat3_to_bullet(
	mat3 const &);
}
}

#endif
