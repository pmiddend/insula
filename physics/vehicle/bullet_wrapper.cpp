#include "bullet_wrapper.hpp"
#include <fcppt/io/cout.hpp>

insula::physics::vehicle::bullet_wrapper::bullet_wrapper(
	btVehicleTuning const &tuning,
	btRigidBody * const chassis,
	btVehicleRaycaster * const raycaster,
	// DIFF: btRaycastVehicle gets just the above three parameters
	scalar const _track_connect_accel)
:
	btRaycastVehicle(
		tuning,
		chassis,
		raycaster),
	track_connect_accel_(
		_track_connect_accel)
{
}

void 
insula::physics::vehicle::bullet_wrapper::updateVehicle(
	btScalar step)
{
	{
		for (int i=0;i<getNumWheels();i++)
		{
			updateWheelTransform(i,false);
		}
	}

	// DIFF BEGIN
	// The vehicle speed is private
	//m_currentVehicleSpeedKmHour = btScalar(3.6) * getRigidBody()->getLinearVelocity().length();
	// DIFF END
	
	const btTransform& chassisTrans = getChassisWorldTransform();

	// DIFF BEGIN
	// m_indexForwardAxis is private
	btVector3 forwardW = getForwardVector();
	/*
	btVector3 forwardW (
		chassisTrans.getBasis()[0][getForwardAxis()],
		chassisTrans.getBasis()[1][getForwardAxis()],
		chassisTrans.getBasis()[2][getForwardAxis()]);
	*/
	// DIFF END

	// DIFF BEGIN
	// The vehicle speed is private
	//if (forwardW.dot(getRigidBody()->getLinearVelocity()) < btScalar(0.))
	//{
	//	m_currentVehicleSpeedKmHour *= btScalar(-1.);
	//}
	// DIFF END

	//
	// simulate suspension
	//
	
	int i=0;
	for (i=0;i<m_wheelInfo.size();i++)
	{
		btScalar depth; 
		depth = rayCast( m_wheelInfo[i]);
	}

	/* DIFF BEGIN */
	// This has been added in addition to stk
	btAssert(getNumWheels() == 4);

	// Work around: make sure that either both wheels on one axis
	// are on ground, or none of them. This avoids the problem of
	// the kart suddenly getting additional angular velocity because
	// e.g. only one rear wheel is on the ground.
  for(int i = 0; i < 4; i+=2)
  {
		if(m_wheelInfo[i].m_raycastInfo.m_isInContact && !(m_wheelInfo[i+1].m_raycastInfo.m_isInContact))
			m_wheelInfo[i+1].m_raycastInfo = m_wheelInfo[i].m_raycastInfo;
		if(!(m_wheelInfo[i].m_raycastInfo.m_isInContact) && m_wheelInfo[i+1].m_raycastInfo.m_isInContact)
		m_wheelInfo[i].m_raycastInfo = m_wheelInfo[i+1].m_raycastInfo;
  }
	/* DIFF END */

	updateSuspension(step);

	
	for (i=0;i<m_wheelInfo.size();i++)
	{
		//apply suspension force
		btWheelInfo& wheel = m_wheelInfo[i];
		
		btScalar suspensionForce = wheel.m_wheelsSuspensionForce;
		
		if (suspensionForce > wheel.m_maxSuspensionForce)
		{
			suspensionForce = wheel.m_maxSuspensionForce;
		}
		btVector3 impulse = wheel.m_raycastInfo.m_contactNormalWS * suspensionForce * step;
		btVector3 relpos = wheel.m_raycastInfo.m_contactPointWS - getRigidBody()->getCenterOfMassPosition();
		
		getRigidBody()->applyImpulse(impulse, relpos);
	
	}
	

	
	updateFriction( step);

	
	for (i=0;i<m_wheelInfo.size();i++)
	{
		btWheelInfo& wheel = m_wheelInfo[i];
		btVector3 relpos = wheel.m_raycastInfo.m_hardPointWS - getRigidBody()->getCenterOfMassPosition();
		btVector3 vel = getRigidBody()->getVelocityInLocalPoint( relpos );

		if (wheel.m_raycastInfo.m_isInContact)
		{
			const btTransform&	chassisWorldTransform = getChassisWorldTransform();

			// DIFF BEGIN
			// m_indexForwardAxis is private
			btVector3 fwd = getForwardVector();
			/*
			btVector3 fwd (
				chassisWorldTransform.getBasis()[0][m_indexForwardAxis],
				chassisWorldTransform.getBasis()[1][m_indexForwardAxis],
				chassisWorldTransform.getBasis()[2][m_indexForwardAxis]);
			*/
			// DIFF END

			btScalar proj = fwd.dot(wheel.m_raycastInfo.m_contactNormalWS);
			fwd -= wheel.m_raycastInfo.m_contactNormalWS * proj;

			btScalar proj2 = fwd.dot(vel);
			
			wheel.m_deltaRotation = (proj2 * step) / (wheel.m_wheelsRadius);
			wheel.m_rotation += wheel.m_deltaRotation;

		} else
		{
			wheel.m_rotation += wheel.m_deltaRotation;
		}
		
		wheel.m_deltaRotation *= btScalar(0.99);//damping of rotation when not in contact

	}



}

/*
void
insula::physics::vehicle::bullet_wrapper::updateFriction(
	btScalar deltaTime)
{
	
}
*/


void
insula::physics::vehicle::bullet_wrapper::updateSuspension(
	btScalar deltaTime)
{
	//fcppt::io::cout << "updating suspension\n";
	(void)deltaTime;

	// DIFF BEGIN
	// m_ChassisBody is private
	btScalar chassisMass = btScalar(1.) / getRigidBody()->getInvMass();
	// DIFF END
	
	for (int w_it=0; w_it<getNumWheels(); w_it++)
	{
		btWheelInfo &wheel_info = m_wheelInfo[w_it];
		
		if ( wheel_info.m_raycastInfo.m_isInContact )
		{
			btScalar force;
			//	Spring
			{
				btScalar	susp_length			= wheel_info.getSuspensionRestLength();
				btScalar	current_length = wheel_info.m_raycastInfo.m_suspensionLength;

				btScalar length_diff = (susp_length - current_length);

				force = wheel_info.m_suspensionStiffness
					* length_diff * wheel_info.m_clippedInvContactDotSuspension;
			}
		
			// Damper
			{
				btScalar projected_rel_vel = wheel_info.m_suspensionRelativeVelocity;
				{
					btScalar	susp_damping;
					if ( projected_rel_vel < btScalar(0.0) )
					{
						susp_damping = wheel_info.m_wheelsDampingCompression;
					}
					else
					{
						susp_damping = wheel_info.m_wheelsDampingRelaxation;
					}
					force -= susp_damping * projected_rel_vel;
				}
			}

			// RESULT
			wheel_info.m_wheelsSuspensionForce = force * chassisMass;
			if (wheel_info.m_wheelsSuspensionForce < btScalar(0.))
			{
				wheel_info.m_wheelsSuspensionForce = btScalar(0.);
			}
		}
		else
		{
			// DIFF BEGIN
			//wheel_info.m_wheelsSuspensionForce = btScalar(0.0);

			// A very unphysical thing to handle slopes that are a bit too steep
			// or uneven (resulting in only one wheel on the ground)
			// If only the front or only the rear wheels are on the ground, add
			// a force pulling the axis down (towards the ground). Note that it
			// is already guaranteed that either both or no wheels on one axis
			// are on the ground, so we have to test only one of the wheels
			wheel_info.m_wheelsSuspensionForce = -track_connect_accel_*chassisMass;
			// DIFF END
		}
	}

}
