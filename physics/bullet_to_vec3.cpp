#include "bullet_to_vec3.hpp"
#include <LinearMath/btVector3.h>

insula::physics::vec3 const
insula::physics::bullet_to_vec3(
	btVector3 const &v)
{
	return vec3(v.getX(),v.getY(),v.getZ());
}
