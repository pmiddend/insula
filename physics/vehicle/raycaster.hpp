#ifndef INSULA_PHYSICS_VEHICLE_RAYCASTER_HPP_INCLUDED
#define INSULA_PHYSICS_VEHICLE_RAYCASTER_HPP_INCLUDED

#include <LinearMath/btVector3.h>
#include "raycast_results.hpp"

namespace insula
{
namespace physics
{
namespace vehicle
{
class raycaster
{
public:
	virtual raycast_results const 
	cast_ray(
		btVector3 const &from,
		btVector3 const &to) = 0;

	virtual ~raycaster();
};
}
}
}

#endif
