#include "upright_constraint.hpp"
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btTransformUtil.h>
#include <LinearMath/btScalar.h>
#include <fcppt/math/pi.hpp>
//#include <fcppt/io/cout.hpp>
#include <cmath>
#include <cstddef>

insula::physics::upright_constraint::upright_constraint(
	btRigidBody &body, 
	btTransform const &_frame_in_a)
:
	btTypedConstraint(
		D6_CONSTRAINT_TYPE,
		body), 
	frame_in_a_(
		_frame_in_a),
	jac_ang_(),
	time_step_(),
	erp_(
		static_cast<btScalar>(1)),
	bounce_(
		static_cast<btScalar>(0)),
	damping_(
		static_cast<btScalar>(1)),
	max_limit_force_(
		static_cast<btScalar>(3000)),
	//	static_cast<btScalar>(30)),
	limit_softness_(
		static_cast<btScalar>(1)),
	//	static_cast<btScalar>(0)),
	disable_time_(
		static_cast<btScalar>(0))
{
	limit_[0].accumulated_impulse_ = 
		static_cast<btScalar>(0);
	limit_[1].accumulated_impulse_ = 
		static_cast<btScalar>(0);
	limit_[0].axis_	= 
		btVector3(
			static_cast<btScalar>(1),
			static_cast<btScalar>(0),
			static_cast<btScalar>(0));
	limit_[1].axis_	= 
		btVector3(
			static_cast<btScalar>(0),
			static_cast<btScalar>(0),
			static_cast<btScalar>(1));
	limit( 
		fcppt::math::pi<btScalar>() * 
		static_cast<btScalar>(0.2));
}

void 
insula::physics::upright_constraint::limit( 
	btScalar const range)
{
	lo_limit_ = -range;
	hi_limit_ = +range;
}

void 
insula::physics::upright_constraint::erp(
	btScalar const _erp)
{
	erp_ = _erp;
}

void 
insula::physics::upright_constraint::bounce( 
	btScalar const _bounce)
{
	bounce_ = _bounce;
}

void 
insula::physics::upright_constraint::max_limit_force( 
	btScalar const _max_limit_force)
{
	max_limit_force_ = _max_limit_force;
}

void 
insula::physics::upright_constraint::limit_softness( 
	btScalar const _limit_softness)
{
	limit_softness_ = _limit_softness;
}

void 
insula::physics::upright_constraint::damping( 
	btScalar const _damping)
{
	damping_ = _damping;
}

void 
insula::physics::upright_constraint::disable_time( 
	btScalar const _disable_time)
{
	disable_time_ = _disable_time;
}

void 
insula::physics::upright_constraint::buildJacobian()
{
	//fcppt::io::cout << "buildJacobian\n";
	btTransform const world_transform = 
		m_rbA.getCenterOfMassTransform() * 
		frame_in_a_;

	btVector3 const up_axis = 
		world_transform.getBasis().getColumn(
			1);

	limit_[0].angle_ = 
		-std::atan2(
			up_axis.getY(), 
			up_axis.getZ())+
		fcppt::math::pi<btScalar>()/static_cast<btScalar>(2);

	limit_[1].angle_ = 
		std::atan2( 
			up_axis.getY(), 
			up_axis.getX())-
		fcppt::math::pi<btScalar>()/static_cast<btScalar>(2);

	for (std::size_t i = 0; i < 2; i++ )
	{
		if (limit_[i].angle_ < -fcppt::math::pi<btScalar>())
			limit_[i].angle_ += 
				static_cast<btScalar>(2) * 
				fcppt::math::pi<btScalar>();

		if (limit_[i].angle_ > fcppt::math::pi<btScalar>())
			limit_[i].angle_ -= 
				static_cast<btScalar>(2) * 
				fcppt::math::pi<btScalar>();

		jac_ang_[i] = 
			btJacobianEntry(  
				limit_[i].axis_,
				m_rbA.getCenterOfMassTransform().getBasis().transpose(),
				m_rbB.getCenterOfMassTransform().getBasis().transpose(),
				m_rbA.getInvInertiaDiagLocal(),
				m_rbB.getInvInertiaDiagLocal());
	}
}

void 
insula::physics::upright_constraint::solveConstraintObsolete(
	btRigidBody &,
	btRigidBody &,
	btScalar const _time_step)
{
	time_step_ = _time_step;

	/*
	// Update disable time and return if constraint is still disabled
	if(disable_time_> static_cast<btScalar>(0))
	{
		disable_time_ -= time_step_;
		if(disable_time_ > static_cast<btScalar>(0)) 
			return;
	}
	*/

	solveAngularLimit( 
		limit_[0], 
		time_step_, 
		static_cast<btScalar>(1) / jac_ang_[0].getDiagonal(), 
		m_rbA);

	solveAngularLimit( 
		limit_[1], 
		time_step_, 	
		static_cast<btScalar>(1) / jac_ang_[1].getDiagonal(), 
		m_rbA);
}

void 
insula::physics::upright_constraint::getInfo1(
	btConstraintInfo1 *info)
{
	info->m_numConstraintRows = 0;
	info->nub = 0;
}

void 
insula::physics::upright_constraint::getInfo2(
	btConstraintInfo2 *)
{
}

 
void
insula::physics::upright_constraint::setParam(
	int const num,
	btScalar const value,
	int axis)
{
	switch(num)
	{
		case BT_CONSTRAINT_ERP:
		case BT_CONSTRAINT_STOP_ERP:
			if((axis >= 0) && (axis < 3))
				erp_ = value;
			else
				bias_factor_ = value;
      break;
    case BT_CONSTRAINT_CFM:
    case BT_CONSTRAINT_STOP_CFM:
      if((axis >= 0) && (axis < 3))
				cfm_ = value;
      else
        ang_cfm_ = value;
      break;
  }
}

btScalar 
insula::physics::upright_constraint::getParam(
	int num,
	int axis) const
{
  btScalar retVal = 
		static_cast<btScalar>(0);
  switch(num)
  {
    case BT_CONSTRAINT_ERP:
    case BT_CONSTRAINT_STOP_ERP:
      if((axis >= 0) && (axis < 3))
        retVal = erp_;
      else if((axis >= 3) && (axis < 6))
				retVal = bias_factor_;
      break;
    case BT_CONSTRAINT_CFM:
    case BT_CONSTRAINT_STOP_CFM:
      if((axis >= 0) && (axis < 3))
        retVal = cfm_;
      else if((axis >= 3) && (axis < 6))
        retVal = cfm_;
      break;
  }
  return retVal;
}

void 
insula::physics::upright_constraint::solveAngularLimit(
	limit_params &_limit,
	btScalar const time_step, 
	btScalar const jacDiagABInv,
	btRigidBody &body0)
{
	// Work out if limit is violated
	if(_limit.angle_ >= lo_limit_ && _limit.angle_ <= hi_limit_) 
		return;

	_limit.current_limit_error_ = 
		_limit.angle_< lo_limit_ 
		? 
			_limit.angle_ - lo_limit_ 
		: 
			_limit.angle_ - hi_limit_;

	btScalar const 
		target_velocity = 
			-erp_ * _limit.current_limit_error_ /
			//(fcppt::math::pi<btScalar>()/static_cast<btScalar>(8)*time_step);
			time_step;

	btScalar max_motor_force = max_limit_force_;

	max_motor_force *= time_step;

	// current velocity difference
	btVector3 const angular_velocity = 
		body0.getAngularVelocity();
	btScalar const axis_angular_velocity = 
		_limit.axis_.dot( 
			angular_velocity);
 
	// correction velocity
	btScalar const motor_velocity = 
		limit_softness_*(target_velocity - damping_ * axis_angular_velocity);

  // correction impulse
	btScalar const unclipped_motor_impulse = 
		(static_cast<btScalar>(1)+bounce_)*
		motor_velocity*
		jacDiagABInv;

	// clip correction impulse
	btScalar clipped_motor_impulse = unclipped_motor_impulse;

	//todo: should clip against accumulated impulse

	if (unclipped_motor_impulse > static_cast<btScalar>(0))
	{
		clipped_motor_impulse = 
			unclipped_motor_impulse > max_motor_force 
			? 
				max_motor_force
			: 
				unclipped_motor_impulse;
	}
	else
	{
		clipped_motor_impulse = 
			unclipped_motor_impulse < -max_motor_force 
			? 
				-max_motor_force
			: 
				unclipped_motor_impulse;
	}

	// sort with accumulated impulses
	btScalar const 
		lo = static_cast<btScalar>(-1e30),
		hi = static_cast<btScalar>(1e30);

	btScalar const old_accum_impulse = 
		_limit.accumulated_impulse_;

	btScalar const sum = 
		old_accum_impulse + clipped_motor_impulse;

	_limit.accumulated_impulse_ = 
		sum > hi 
		? 
			static_cast<btScalar>(0) 
		: 
			sum < lo 
			? 
				static_cast<btScalar>(0) 
			: 
				sum;

	clipped_motor_impulse = 
		_limit.accumulated_impulse_ - old_accum_impulse;

	btVector3 const motor_imp = 
		clipped_motor_impulse * _limit.axis_;
    body0.applyTorqueImpulse(
			motor_imp);
}
