#include "transform_from_vec3.hpp"
#include "vec3_to_bullet.hpp"

btTransform const
insula::physics::transform_from_vec3(
	vec3 const &v)
{
	return 
		btTransform(
			btMatrix3x3::getIdentity(),
			vec3_to_bullet(
				v));
}
