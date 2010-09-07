#ifndef INSULA_PHYSICS_VEHICLE_FRICTION_CONSTRAINT_HPP_INCLUDED
#define INSULA_PHYSICS_VEHICLE_FRICTION_CONSTRAINT_HPP_INCLUDED

#include "../scalar.hpp"
#include <BulletDynamics/ConstraintSolver/btTypedConstraint.h>

class btRaycastVehicle;

namespace insula
{
namespace physics
{
namespace vehicle
{
class friction_constraint
:
	public btTypedConstraint
{
public:
	explicit
	friction_constraint(
		btRaycastVehicle &);

	// @override
	void 
	getInfo1(
		btConstraintInfo1*);

	// @override
	void 
	getInfo2(
		btConstraintInfo2*);

	// @override
	void
	setParam(
		int num, 
		btScalar value, 
		int axis = -1);

	// @override
	btScalar 
	getParam(
		int num, 
		int axis = -1) const;
private:
	btRaycastVehicle &vehicle_;
	scalar cfm_,erp_,bias_factor_,ang_cfm_;
};
}
}
}

#endif
