#ifndef INSULA_PHYSICS_BULLET_TO_VEC3_HPP_INCLUDED
#define INSULA_PHYSICS_BULLET_TO_VEC3_HPP_INCLUDED

#include "vec3.hpp"

class btVector3;

namespace insula
{
namespace physics
{
vec3 const
bullet_to_vec3(
	btVector3 const &);
}
}

#endif
