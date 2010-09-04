#ifndef INSULA_PHYSICS_VEHICLE_BULLET_WRAPPER_HPP_INCLUDED
#define INSULA_PHYSICS_VEHICLE_BULLET_WRAPPER_HPP_INCLUDED

#include "../scalar.hpp"
#include <BulletDynamics/Vehicle/btRaycastVehicle.h>

namespace insula
{
namespace physics
{
namespace vehicle
{
/*
	We want to extend btRaycastVehicle, but it's not really suitable for
	just deriving from it and adding functionality. So we have to
	copy-paste most of it.
 */
class bullet_wrapper
:
	public btRaycastVehicle
{
public:
	explicit
	bullet_wrapper(
		btVehicleTuning const &,
		btRigidBody *chassis,
		btVehicleRaycaster *raycaster,
		// DIFF: btRaycastVehicle gets just the above three parameters
		scalar track_connect_accel);

	// NOTE: supertuxkart copied the rayCast method and changed
	// absolutely nothing. I'm skipping that.
	// void rayCast(...)

	// virtual function in btRaycastVehicle
	void 
	updateVehicle(
		btScalar step);

	// This is a non-virtual function in btRaycastVehicle, it's ok,
	// however, because it is only called by updateVehicle, which we
	// have overidden.
	void 
	updateSuspension(
		btScalar step);

private:
	scalar track_connect_accel_;
};
}
}
}

#endif
