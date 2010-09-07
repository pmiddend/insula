#include "friction_constraint.hpp"
#include "../scalar.hpp"
#include <BulletDynamics/Vehicle/btRaycastVehicle.h>
#include <fcppt/assert.hpp>

insula::physics::vehicle::friction_constraint::friction_constraint(
	btRaycastVehicle &_vehicle)
:
	btTypedConstraint(
		D6_CONSTRAINT_TYPE,
		*_vehicle.getRigidBody()),
	vehicle_(
		_vehicle),
	cfm_(
		static_cast<scalar>(1)),
	erp_(
		static_cast<scalar>(1)),
	// Don't they have to be initialized?
	bias_factor_(),
	ang_cfm_()
{
}

void 
insula::physics::vehicle::friction_constraint::getInfo1(
	btConstraintInfo1 * const info)
{
	FCPPT_ASSERT(
		info);

	// Add two constraint rows for each wheel on the ground

	info->m_numConstraintRows = 0;
	
	for (int i = 0; i < vehicle_.getNumWheels(); ++i)
	{
		btWheelInfo const& wheel_info = 
			vehicle_.getWheelInfo(i);
		info->m_numConstraintRows += 
			2 * (wheel_info.m_raycastInfo.m_groundObject != 0);
	}
}

void 
insula::physics::vehicle::friction_constraint::getInfo2(
	btConstraintInfo2 * const info)
{
	FCPPT_ASSERT(
		info);

	btRigidBody const &chassis = 
		*vehicle_.getRigidBody();

	int row = 0;

	// Setup sideways friction.
	for (int i = 0; i < vehicle_.getNumWheels(); ++i)
	{
		btWheelInfo const &wheel_info = 
			vehicle_.getWheelInfo(
				i);

		// Only if the wheel is on the ground:
		if (!wheel_info.m_raycastInfo.m_groundObject)
			continue;

		int const row_index = 
			row++ * info->rowskip;

		// Set axis to be the direction of motion:
		btVector3 const &axis = 
			wheel_info.m_raycastInfo.m_wheelAxleWS;

		info->m_J1linearAxis[row_index] = axis[0];
		info->m_J1linearAxis[row_index+1] = axis[1];
		info->m_J1linearAxis[row_index+2] = axis[2];

		// Set angular axis.
		btVector3 const rel_pos = 
			wheel_info.m_raycastInfo.m_contactPointWS - 
			chassis.getCenterOfMassPosition();

		btVector3 rel_pos_cross = 
			rel_pos.cross(
				axis);

		info->m_J1angularAxis[row_index] = rel_pos_cross[0];
		info->m_J1angularAxis[row_index+1] = rel_pos_cross[1];
		info->m_J1angularAxis[row_index+2] = rel_pos_cross[2];

		// Set constraint error (target relative velocity = 0.0)
 		info->m_constraintError[row_index] = static_cast<scalar>(0);
		// Set constraint force mixing
		info->cfm[row_index] = cfm_; 

		// Set maximum friction force according to Coulomb's law
		// Substitute Pacejka here
		btScalar const max_friction = 
			wheel_info.m_wheelsSuspensionForce * 
			wheel_info.m_frictionSlip / 
			info->fps;

		// Set friction limits.
		info->m_lowerLimit[row_index] = -max_friction;
		info->m_upperLimit[row_index] = max_friction;
	}

	// Setup forward friction.
	for (int i = 0; i < vehicle_.getNumWheels(); ++i)
	{
		btWheelInfo const &wheel_info = vehicle_.getWheelInfo(i);

		// Only if the wheel is on the ground:
		if (!wheel_info.m_raycastInfo.m_groundObject)
			continue;

		int const row_index = 
			row++ * info->rowskip;

		// Set axis to be the direction of motion:
		btVector3 const axis = 
			wheel_info.m_raycastInfo.m_wheelAxleWS.cross(
				wheel_info.m_raycastInfo.m_wheelDirectionWS);

		info->m_J1linearAxis[row_index] = axis[0];
		info->m_J1linearAxis[row_index+1] = axis[1];
		info->m_J1linearAxis[row_index+2] = axis[2];

		// Set angular axis.
		btVector3 const rel_pos = 
			wheel_info.m_raycastInfo.m_contactPointWS - 
			chassis.getCenterOfMassPosition();

		btVector3 const rel_pos_cross = 
			rel_pos.cross(
				axis);

		info->m_J1angularAxis[row_index] = axis[0];
		info->m_J1angularAxis[row_index+1] = axis[1];
		info->m_J1angularAxis[row_index+2] = axis[2];

		// FIXME: Calculate the speed of the contact point on the wheel spinning.
		// Estimate the wheel's angular velocity = m_deltaRotation

		btScalar const wheel_velocity = 
			wheel_info.m_deltaRotation * 
			wheel_info.m_wheelsRadius;

		// Set constraint error (target relative velocity = 0.0)
		info->m_constraintError[row_index] = wheel_velocity;
		info->cfm[row_index] = cfm_; // Set constraint force mixing

		// Set maximum friction force
		btScalar const max_friction = 
			wheel_info.m_wheelsSuspensionForce * 
			wheel_info.m_frictionSlip / 
			info->fps;

		// Set friction limits.
		info->m_lowerLimit[row_index] = -max_friction;
		info->m_upperLimit[row_index] =  max_friction;
	}
}

// @override
void
insula::physics::vehicle::friction_constraint::setParam(
	int num, 
	btScalar value, 
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

// @override
btScalar 
insula::physics::vehicle::friction_constraint::getParam(
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
