#include "mat3_to_bullet.hpp"
#include <fcppt/math/matrix/basic_impl.hpp>

btMatrix3x3 const
insula::physics::mat3_to_bullet(
	mat3 const &m)
{
	return 
		btMatrix3x3(
			m[0][0],m[0][1],m[0][2],
			m[1][0],m[1][1],m[1][2],
			m[2][0],m[2][1],m[2][2]);
}
