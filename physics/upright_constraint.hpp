#ifndef INSULA_PHYSICS_UPRIGHT_CONSTRAINT_HPP_INCLUDED
#define INSULA_PHYSICS_UPRIGHT_CONSTRAINT_HPP_INCLUDED

#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <BulletDynamics/ConstraintSolver/btJacobianEntry.h>
#include <BulletDynamics/ConstraintSolver/btTypedConstraint.h>
#include <array>

class btRigidBody;

namespace insula
{
namespace physics
{
class upright_constraint 
: 
	public btTypedConstraint
{
public:
	class limit_params
	{
	public:
		btVector3   axis_;
		btScalar    angle_;
		btScalar    accumulated_impulse_;
		btScalar    current_limit_error_;
	};

	upright_constraint(upright_constraint const &) = delete;
	upright_constraint &operator=(upright_constraint const &) = delete;

	explicit
	upright_constraint(
		btRigidBody& rbA, 
		btTransform const &frameInA);

	// -PI,+PI is the full range
	// 0,0 is no rotation around x or z
	// -PI*0.2,+PI*0.2 is a nice bit of tilt
	void 
	limit( 
		btScalar const range);

	// Error correction scaling
	// 0 - 1
	void 
	erp(
		btScalar erp);

	void 
	bounce( 
		btScalar bounce);

	void 
	max_limit_force( 
		btScalar _force);

	void 
	limit_softness( 
		btScalar softness);

	void 
	damping( 
		btScalar damping );

	void 
	disable_time( 
		btScalar t);

	void  
	buildJacobian();

	void  
	solveConstraintObsolete(
		btRigidBody &,
		btRigidBody &,
		btScalar time_step);

	void 
	getInfo1(
		btConstraintInfo1*);

	void 
	getInfo2(
		btConstraintInfo2 *);

	void 
	setParam(
		int num, 
		btScalar value, 
		int axis = -1);

	///return the local value of parameter
	btScalar 
	getParam(
		int num, 
		int axis = -1) const;
private:
	btTransform frame_in_a_;
	std::array<btJacobianEntry,2> jac_ang_;

	btScalar time_step_;
	btScalar erp_;
	btScalar cfm_;
	btScalar ang_cfm_;
	btScalar bias_factor_;
	btScalar bounce_;
	btScalar damping_;
	btScalar max_limit_force_;
	btScalar limit_softness_;
	btScalar hi_limit_;
	btScalar lo_limit_;
	btScalar disable_time_;

	std::array<limit_params,2> limit_;

	void 
	solveAngularLimit(
		limit_params &,
		btScalar time_step, 
		btScalar jacDiagABInv,
		btRigidBody &body0);
};
}
}

#endif

