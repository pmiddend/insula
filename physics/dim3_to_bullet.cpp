#include "dim3_to_bullet.hpp"
#include <fcppt/math/dim/basic_impl.hpp>

btVector3 const
insula::physics::dim3_to_bullet(
	dim3 const &d)
{
	return btVector3(d[0],d[1],d[2]);
}
