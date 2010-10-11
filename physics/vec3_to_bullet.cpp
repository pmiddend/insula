#include "vec3_to_bullet.hpp"

btVector3 const
insula::physics::vec3_to_bullet(
	vec3 const &v)
{
	return btVector3(v[0],v[1],v[2]);
}
