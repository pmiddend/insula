#include "bullet_to_mat4.hpp"
#include <LinearMath/btMatrix3x3.h>

insula::physics::mat4 const
insula::physics::bullet_to_mat4(
	btMatrix3x3 const &m)
{
	return 
		mat4(
			m[0].getX(),m[0].getY(),m[0].getZ(),0,
			m[1].getX(),m[1].getY(),m[1].getZ(),0,
			m[2].getX(),m[2].getY(),m[2].getZ(),0,
			0,0,0,1);
}
