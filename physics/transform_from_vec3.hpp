#ifndef INSULA_PHYSICS_TRANSFORM_FROM_VEC3_HPP_INCLUDED
#define INSULA_PHYSICS_TRANSFORM_FROM_VEC3_HPP_INCLUDED

#include "vec3.hpp"
#include <LinearMath/btTransform.h>

namespace insula
{
namespace physics
{
btTransform const
transform_from_vec3(
	vec3 const &);
}
}

#endif
