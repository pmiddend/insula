#ifndef INSULA_PHYSICS_VECTOR_TO_BULLET_HPP_INCLUDED
#define INSULA_PHYSICS_VECTOR_TO_BULLET_HPP_INCLUDED

#include "vec3.hpp"
#include <LinearMath/btVector3.h>

namespace insula
{
namespace physics
{
btVector3 const
vec3_to_bullet(
	vec3 const &);
}
}

#endif
