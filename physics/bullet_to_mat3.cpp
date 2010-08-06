#include "bullet_to_mat3.hpp"
#include <LinearMath/btMatrix3x3.h>

insula::physics::mat3 const
insula::physics::bullet_to_mat3(
	btMatrix3x3 const &m)
{
	return 
		mat3(
			m[0].getX(),m[0].getY(),m[0].getZ(),
			m[1].getX(),m[1].getY(),m[1].getZ(),
			m[2].getX(),m[2].getY(),m[2].getZ());
}
