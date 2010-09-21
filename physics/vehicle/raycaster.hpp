#ifndef INSULA_PHYSICS_VEHICLE_RAYCASTER_HPP_INCLUDED
#define INSULA_PHYSICS_VEHICLE_RAYCASTER_HPP_INCLUDED

#include "../../height_map/object_fwd.hpp"
#include <BulletDynamics/Vehicle/btVehicleRaycaster.h>

class btDynamicsWorld;

namespace insula
{
namespace physics
{
namespace vehicle
{
class raycaster
:
	public btVehicleRaycaster
{
public:
	explicit
	raycaster(
		btDynamicsWorld &,
		height_map::object const &);

	void* 
	castRay(
		btVector3 const &from,
		btVector3 const &to, 
		btVehicleRaycasterResult &result);
private:
	btDynamicsWorld &world_;
	height_map::object const &height_map_;
};
}
}
}

#endif
