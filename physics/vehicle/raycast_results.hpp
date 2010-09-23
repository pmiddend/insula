#ifndef INSULA_PHYSICS_VEHICLE_RAYCAST_RESULTS_HPP_INCLUDED
#define INSULA_PHYSICS_VEHICLE_RAYCAST_RESULTS_HPP_INCLUDED

#include <LinearMath/btVector3.h>

namespace insula
{
namespace physics
{
namespace vehicle
{
class raycast_results
{
public:
	btVector3 point;
	btVector3 normal;
	void *object;
	btScalar dist_fraction;
	
	explicit
	raycast_results()
	:
		object(
			0)
	{
	}
};
}
}
}

#endif
