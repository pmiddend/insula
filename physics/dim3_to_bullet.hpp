#ifndef INSULA_PHYSICS_DIM3_TO_BULLET_HPP_INCLUDED
#define INSULA_PHYSICS_DIM3_TO_BULLET_HPP_INCLUDED

#include "dim3.hpp"
#include <LinearMath/btVector3.h>

namespace insula
{
namespace physics
{
btVector3 const
dim3_to_bullet(
	dim3 const &);
}
}

#endif
