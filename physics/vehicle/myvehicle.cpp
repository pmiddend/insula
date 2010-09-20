#include "myvehicle.hpp"
#include <LinearMath/btVector3.h>

#include <BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.h>
#include <BulletDynamics/ConstraintSolver/btJacobianEntry.h>
#include <LinearMath/btQuaternion.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletDynamics/Vehicle/btVehicleRaycaster.h>
#include <BulletDynamics/Vehicle/btWheelInfo.h>
#include <LinearMath/btMinMax.h>
#include <LinearMath/btIDebugDraw.h>
#include <BulletDynamics/ConstraintSolver/btContactConstraint.h>
#include <cmath>

btScalar const magic_zero_constant = static_cast<btScalar>(0.0001);


myvehicle::myvehicle(const btVehicleTuning& tuning,btRigidBody* chassis,	btVehicleRaycaster* raycaster )
:m_vehicleRaycaster(raycaster),
m_pitchControl(btScalar(0.))
{
	m_chassisBody = chassis;
	m_indexRightAxis = 0;
	m_indexUpAxis = 2;
	m_indexForwardAxis = 1;
	defaultInit(tuning);
}


void myvehicle::defaultInit(const btVehicleTuning& tuning)
{
	(void)tuning;
	m_currentVehicleSpeedKmHour = btScalar(0.);
	m_steeringValue = btScalar(0.);
	
}

	

myvehicle::~myvehicle()
{
}


//
// basically most of the code is general for 2 or 4 wheel vehicles, but some of it needs to be reviewed
//
btWheelInfo&	myvehicle::addWheel( const btVector3& connectionPointCS, const btVector3& wheelDirectionCS0,const btVector3& wheelAxleCS, btScalar suspensionRestLength, btScalar wheelRadius,const btVehicleTuning& tuning, bool isFrontWheel)
{

	btWheelInfoConstructionInfo ci;

	ci.m_chassisConnectionCS = connectionPointCS;
	ci.m_wheelDirectionCS = wheelDirectionCS0;
	ci.m_wheelAxleCS = wheelAxleCS;
	ci.m_suspensionRestLength = suspensionRestLength;
	ci.m_wheelRadius = wheelRadius;
	ci.m_suspensionStiffness = tuning.m_suspensionStiffness;
	ci.m_wheelsDampingCompression = tuning.m_suspensionCompression;
	ci.m_wheelsDampingRelaxation = tuning.m_suspensionDamping;
	ci.m_frictionSlip = tuning.m_frictionSlip;
	ci.m_bIsFrontWheel = isFrontWheel;
	ci.m_maxSuspensionTravelCm = tuning.m_maxSuspensionTravelCm;
	ci.m_maxSuspensionForce = tuning.m_maxSuspensionForce;

	m_wheelInfo.push_back( btWheelInfo(ci));
	
	btWheelInfo& wheel = m_wheelInfo[getNumWheels()-1];
	
	updateWheelTransformsWS( wheel , false );
	updateWheelTransform(getNumWheels()-1,false);
	return wheel;
}




const btTransform&	myvehicle::getWheelTransformWS( int wheelIndex ) const
{
	btAssert(wheelIndex < getNumWheels());
	const btWheelInfo& wheel = m_wheelInfo[wheelIndex];
	return wheel.m_worldTransform;

}

void	myvehicle::updateWheelTransform( int wheelIndex , bool interpolatedTransform)
{
	
	btWheelInfo& wheel = m_wheelInfo[ wheelIndex ];
	updateWheelTransformsWS(wheel,interpolatedTransform);
	btVector3 up = -wheel.m_raycastInfo.m_wheelDirectionWS;
	const btVector3& right = wheel.m_raycastInfo.m_wheelAxleWS;
	btVector3 fwd = up.cross(right);
	fwd = fwd.normalize();
//	up = right.cross(fwd);
//	up.normalize();

	//rotate around steering over de wheelAxleWS
	btScalar steering = wheel.m_steering;
	
	btQuaternion steeringOrn(up,steering);//wheel.m_steering);
	btMatrix3x3 steeringMat(steeringOrn);

	btQuaternion rotatingOrn(right,-wheel.m_rotation);
	btMatrix3x3 rotatingMat(rotatingOrn);

	btMatrix3x3 basis2(
		right[0],fwd[0],up[0],
		right[1],fwd[1],up[1],
		right[2],fwd[2],up[2]
	);
	
	wheel.m_worldTransform.setBasis(steeringMat * rotatingMat * basis2);
	wheel.m_worldTransform.setOrigin(
		wheel.m_raycastInfo.m_hardPointWS + wheel.m_raycastInfo.m_wheelDirectionWS * wheel.m_raycastInfo.m_suspensionLength
	);
}

void myvehicle::resetSuspension()
{

	int i;
	for (i=0;i<m_wheelInfo.size();	i++)
	{
			btWheelInfo& wheel = m_wheelInfo[i];
			wheel.m_raycastInfo.m_suspensionLength = wheel.getSuspensionRestLength();
			wheel.m_suspensionRelativeVelocity = btScalar(0.0);
			
			wheel.m_raycastInfo.m_contactNormalWS = - wheel.m_raycastInfo.m_wheelDirectionWS;
			//wheel_info.setContactFriction(btScalar(0.0));
			wheel.m_clippedInvContactDotSuspension = btScalar(1.0);
	}
}

void	myvehicle::updateWheelTransformsWS(btWheelInfo& wheel , bool interpolatedTransform)
{
	wheel.m_raycastInfo.m_isInContact = false;

	btTransform chassisTrans = getChassisWorldTransform();
	if (interpolatedTransform && (getRigidBody()->getMotionState()))
	{
		getRigidBody()->getMotionState()->getWorldTransform(chassisTrans);
	}

	wheel.m_raycastInfo.m_hardPointWS = chassisTrans( wheel.m_chassisConnectionPointCS );
	wheel.m_raycastInfo.m_wheelDirectionWS = chassisTrans.getBasis() *  wheel.m_wheelDirectionCS ;
	wheel.m_raycastInfo.m_wheelAxleWS = chassisTrans.getBasis() * wheel.m_wheelAxleCS;
}

btScalar myvehicle::rayCast(btWheelInfo& wheel)
{
	updateWheelTransformsWS( wheel,false);

	
	btScalar depth = -1;
	
	btScalar raylen = wheel.getSuspensionRestLength()+wheel.m_wheelsRadius;

	btVector3 rayvector = wheel.m_raycastInfo.m_wheelDirectionWS * (raylen);
	const btVector3& source = wheel.m_raycastInfo.m_hardPointWS;
	wheel.m_raycastInfo.m_contactPointWS = source + rayvector;
	const btVector3& target = wheel.m_raycastInfo.m_contactPointWS;

	btScalar param = btScalar(0.);
	
	btVehicleRaycaster::btVehicleRaycasterResult	rayResults;

	btAssert(m_vehicleRaycaster);

	void* object = m_vehicleRaycaster->castRay(source,target,rayResults);

	wheel.m_raycastInfo.m_groundObject = 0;

	if (object)
	{
		param = rayResults.m_distFraction;
		depth = raylen * rayResults.m_distFraction;
		wheel.m_raycastInfo.m_contactNormalWS  = rayResults.m_hitNormalInWorld;
		wheel.m_raycastInfo.m_isInContact = true;
		
		wheel.m_raycastInfo.m_groundObject = &getFixedBody();///@todo for driving on dynamic/movable objects!;
		//wheel.m_raycastInfo.m_groundObject = object;


		btScalar hitDistance = param*raylen;
		wheel.m_raycastInfo.m_suspensionLength = hitDistance - wheel.m_wheelsRadius;
		//clamp on max suspension travel

		btScalar  minSuspensionLength = wheel.getSuspensionRestLength() - wheel.m_maxSuspensionTravelCm*btScalar(0.01);
		btScalar maxSuspensionLength = wheel.getSuspensionRestLength()+ wheel.m_maxSuspensionTravelCm*btScalar(0.01);
		if (wheel.m_raycastInfo.m_suspensionLength < minSuspensionLength)
		{
			wheel.m_raycastInfo.m_suspensionLength = minSuspensionLength;
		}
		if (wheel.m_raycastInfo.m_suspensionLength > maxSuspensionLength)
		{
			wheel.m_raycastInfo.m_suspensionLength = maxSuspensionLength;
		}

		wheel.m_raycastInfo.m_contactPointWS = rayResults.m_hitPointInWorld;

		btScalar denominator= wheel.m_raycastInfo.m_contactNormalWS.dot( wheel.m_raycastInfo.m_wheelDirectionWS );

		btVector3 chassis_velocity_at_contactPoint;
		btVector3 relpos = wheel.m_raycastInfo.m_contactPointWS-getRigidBody()->getCenterOfMassPosition();

		chassis_velocity_at_contactPoint = getRigidBody()->getVelocityInLocalPoint(relpos);

		btScalar projVel = wheel.m_raycastInfo.m_contactNormalWS.dot( chassis_velocity_at_contactPoint );

		if ( denominator >= btScalar(-0.1))
		{
			wheel.m_suspensionRelativeVelocity = btScalar(0.0);
			wheel.m_clippedInvContactDotSuspension = btScalar(1.0) / btScalar(0.1);
		}
		else
		{
			btScalar inv = btScalar(-1.) / denominator;
			wheel.m_suspensionRelativeVelocity = projVel * inv;
			wheel.m_clippedInvContactDotSuspension = inv;
		}
			
	} else
	{
		//put wheel info as in rest position
		wheel.m_raycastInfo.m_suspensionLength = wheel.getSuspensionRestLength();
		wheel.m_suspensionRelativeVelocity = btScalar(0.0);
		wheel.m_raycastInfo.m_contactNormalWS = - wheel.m_raycastInfo.m_wheelDirectionWS;
		wheel.m_clippedInvContactDotSuspension = btScalar(1.0);
	}

	return depth;
}


const btTransform& myvehicle::getChassisWorldTransform() const
{
	/*if (getRigidBody()->getMotionState())
	{
		btTransform chassisWorldTrans;
		getRigidBody()->getMotionState()->getWorldTransform(chassisWorldTrans);
		return chassisWorldTrans;
	}
	*/

	
	return getRigidBody()->getCenterOfMassTransform();
}


void myvehicle::updateVehicle( btScalar step )
{
	{
		for (int i=0;i<getNumWheels();i++)
		{
			updateWheelTransform(i,false);
		}
	}


	m_currentVehicleSpeedKmHour = btScalar(3.6) * getRigidBody()->getLinearVelocity().length();
	
	const btTransform& chassisTrans = getChassisWorldTransform();

	btVector3 forwardW (
		chassisTrans.getBasis()[0][m_indexForwardAxis],
		chassisTrans.getBasis()[1][m_indexForwardAxis],
		chassisTrans.getBasis()[2][m_indexForwardAxis]);

	if (forwardW.dot(getRigidBody()->getLinearVelocity()) < btScalar(0.))
	{
		m_currentVehicleSpeedKmHour *= btScalar(-1.);
	}

	//
	// simulate suspension
	//
	
	int i=0;
	for (i=0;i<m_wheelInfo.size();i++)
	{
		btScalar depth; 
		depth = rayCast( m_wheelInfo[i]);
	}

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

	
	// Updates the wheel rotation (not important)
	for (i=0;i<m_wheelInfo.size();i++)
	{
		btWheelInfo& wheel = m_wheelInfo[i];
		btVector3 relpos = wheel.m_raycastInfo.m_hardPointWS - getRigidBody()->getCenterOfMassPosition();
		btVector3 vel = getRigidBody()->getVelocityInLocalPoint( relpos );

		if (wheel.m_raycastInfo.m_isInContact)
		{
			const btTransform&	chassisWorldTransform = getChassisWorldTransform();

			btVector3 fwd (
				chassisWorldTransform.getBasis()[0][m_indexForwardAxis],
				chassisWorldTransform.getBasis()[1][m_indexForwardAxis],
				chassisWorldTransform.getBasis()[2][m_indexForwardAxis]);

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


void	myvehicle::setSteeringValue(btScalar steering,int wheel)
{
	btAssert(wheel>=0 && wheel < getNumWheels());

	btWheelInfo& wheelInfo = getWheelInfo(wheel);
	wheelInfo.m_steering = steering;
}



btScalar	myvehicle::getSteeringValue(int wheel) const
{
	return getWheelInfo(wheel).m_steering;
}


void	myvehicle::applyEngineForce(btScalar force, int wheel)
{
	btAssert(wheel>=0 && wheel < getNumWheels());
	btWheelInfo& wheelInfo = getWheelInfo(wheel);
	wheelInfo.m_engineForce = force;
}


const btWheelInfo&	myvehicle::getWheelInfo(int index) const
{
	btAssert((index >= 0) && (index < 	getNumWheels()));
	
	return m_wheelInfo[index];
}

btWheelInfo&	myvehicle::getWheelInfo(int index) 
{
	btAssert((index >= 0) && (index < 	getNumWheels()));
	
	return m_wheelInfo[index];
}

void myvehicle::setBrake(btScalar brake,int wheelIndex)
{
	btAssert((wheelIndex >= 0) && (wheelIndex < 	getNumWheels()));
	getWheelInfo(wheelIndex).m_brake = brake;
}


void	myvehicle::updateSuspension(btScalar deltaTime)
{
	(void)deltaTime;

	btScalar chassisMass = btScalar(1.) / m_chassisBody->getInvMass();
	
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
			wheel_info.m_wheelsSuspensionForce = btScalar(0.0);
		}
	}

}


struct btWheelContactPoint
{
	btRigidBody* m_body0;
	btRigidBody* m_body1;
	btVector3	m_frictionPositionWorld;
	btVector3	m_frictionDirectionWorld;
	btScalar	m_jacDiagABInv;
	btScalar	m_maxImpulse;


	btWheelContactPoint(btRigidBody* body0,btRigidBody* body1,const btVector3& frictionPosWorld,const btVector3& frictionDirectionWorld, btScalar maxImpulse)
		:m_body0(body0),
		m_body1(body1),
		m_frictionPositionWorld(frictionPosWorld),
		m_frictionDirectionWorld(frictionDirectionWorld),
		m_maxImpulse(maxImpulse)
	{
		btScalar denom0 = body0->computeImpulseDenominator(frictionPosWorld,frictionDirectionWorld);
		btScalar denom1 = body1->computeImpulseDenominator(frictionPosWorld,frictionDirectionWorld);
		btScalar	relaxation = 1.f;
		m_jacDiagABInv = relaxation/(denom0+denom1);
	}



};

// Uhm...?
btScalar calcRollingFriction(btWheelContactPoint& contactPoint);
// This is strikingly similar to resolveSingleBilateral (somewhere
// above)
btScalar calcRollingFriction(btWheelContactPoint& contactPoint)
{

	btScalar j1=0.f;

	const btVector3& contactPosWorld = contactPoint.m_frictionPositionWorld;

	btVector3 rel_pos1 = contactPosWorld - contactPoint.m_body0->getCenterOfMassPosition(); 
	btVector3 rel_pos2 = contactPosWorld - contactPoint.m_body1->getCenterOfMassPosition();
	
	btScalar maxImpulse  = contactPoint.m_maxImpulse;
	
	btVector3 vel1 = contactPoint.m_body0->getVelocityInLocalPoint(rel_pos1);
	btVector3 vel2 = contactPoint.m_body1->getVelocityInLocalPoint(rel_pos2);
	btVector3 vel = vel1 - vel2;

	btScalar vrel = contactPoint.m_frictionDirectionWorld.dot(vel);

	// calculate j that moves us to zero relative velocity
	j1 = -vrel * contactPoint.m_jacDiagABInv;
	btSetMin(j1, maxImpulse);
	btSetMax(j1, -maxImpulse);

	return j1;
}




btScalar sideFrictionStiffness2 = btScalar(1.0);
void	myvehicle::updateFriction(btScalar	timeStep)
{

		//calculate the impulse, so that the wheels don't move sidewards
		int numWheel = getNumWheels();
		if (!numWheel)
			return;

		// To calculate the forward and side impulse, we need the wheel
		// forward and right vectors (axle)
		m_forwardWS.resize(numWheel);
		m_axle.resize(numWheel);
		m_forwardImpulse.resize(numWheel);
		m_sideImpulse.resize(numWheel);
	

		// Reset forward and side impulses
		for (int i=0;i<getNumWheels();i++)
		{
			m_sideImpulse[i] = btScalar(0.);
			m_forwardImpulse[i] = btScalar(0.);
		}
	
		{
	
			for (int i=0;i<getNumWheels();i++)
			{

				btWheelInfo& wheelInfo = m_wheelInfo[i];
					
				// Do a safe upcast (since we know that the ground object will
				// be the fixed body?)
				btRigidBody * groundObject = 
					static_cast<btRigidBody *>(
						wheelInfo.m_raycastInfo.m_groundObject);

				if (!groundObject)
					continue;

				btTransform const& wheelTrans = 
					getWheelTransformWS(i);

				// Get wheel forward axis (might be parallel to the vehicle
				// forward axis, that's where it gets interesting)
				btMatrix3x3 wheelBasis0 = wheelTrans.getBasis();
				m_axle[i] = btVector3(	
					wheelBasis0[0][m_indexRightAxis],
					wheelBasis0[1][m_indexRightAxis],
					wheelBasis0[2][m_indexRightAxis]);

				btVector3 const & surfNormalWS = wheelInfo.m_raycastInfo.m_contactNormalWS;
				// The dot product of the surface normal and the wheel axle
				// should be zero in most cases, so we can ignore the code below?
				btScalar proj = m_axle[i].dot(surfNormalWS);
				m_axle[i] -= surfNormalWS * proj;
				m_axle[i] = m_axle[i].normalize();

				// Forward vector is calculated via the axle cross the surface
				// normal
				m_forwardWS[i] = surfNormalWS.cross(m_axle[i]);
				m_forwardWS[i].normalize();

				// This takes both bodies' velocities and calculates the
				// magnitude of the side impulse
				resolveSingleBilateral(
					*m_chassisBody, 
					wheelInfo.m_raycastInfo.m_contactPointWS,
					*groundObject, 
					wheelInfo.m_raycastInfo.m_contactPointWS,
					btScalar(0.), 
					m_axle[i],
					m_sideImpulse[i],
					timeStep);

				m_sideImpulse[i] *= sideFrictionStiffness2;
			}
		}

	btScalar sideFactor = btScalar(1.);
	btScalar fwdFactor = 0.5;

	// The sliding constant is global. One wheel slides => sliding = true
	bool sliding = false;
	{
		// This loop is for rolling friction. 
		// If the vehicle is braking, this will calculate a negative force,
		// if it's accelerating, this will be a positive force,
		// if neither is the case, this will be a negative force (of with
		// later user-controllable magnitude)
		for (int wheel =0;wheel <getNumWheels();wheel++)
		{
			btWheelInfo& wheelInfo = m_wheelInfo[wheel];
			btRigidBody* groundObject = 
				static_cast<btRigidBody *>(
					wheelInfo.m_raycastInfo.m_groundObject);

			btScalar	rollingFriction = 0.f;

			// If the current wheel touches the ground
			if (groundObject)
			{
				// And if there's engine force applied to it
				if (std::abs(wheelInfo.m_engineForce) > magic_zero_constant)
				{
					// Set the rolling friction (the force that resists the
					// wheel going forward) to the engine force (making it an
					// amplifying force)
					rollingFriction = wheelInfo.m_engineForce * timeStep;
				} 
				else
				{
					btScalar defaultRollingFrictionImpulse = 1.f;
					btScalar maxImpulse = 
						std::abs(wheelInfo.m_brake) > magic_zero_constant
						? 
							wheelInfo.m_brake 
						: 
							defaultRollingFrictionImpulse;
					btWheelContactPoint contactPt(
						m_chassisBody,groundObject,
						wheelInfo.m_raycastInfo.m_contactPointWS,
						m_forwardWS[wheel],
						maxImpulse);
					rollingFriction = 
						calcRollingFriction(
							contactPt);
				}
			}

			//switch between active rolling (throttle), braking and non-active rolling friction (no throttle/break)
			



			m_forwardImpulse[wheel] = btScalar(0.);
			m_wheelInfo[wheel].m_skidInfo= btScalar(1.);

			// Determines if the vehicle slides and if so, by how much.
			// Formally, a vehicle slides if
			// |Force pulling down|^2 < |Force pulling forward|^2 + |Force pulling sideways|^2
			if (groundObject)
			{
				m_wheelInfo[wheel].m_skidInfo= btScalar(1.);
				
				// "Maximum" in this case means: If it's above this marker,
				// then the force that pushes the vehicle _down_ is smaller
				// than the force which pushes it forward. In other words, it slides
				btScalar maximp = 
					wheelInfo.m_wheelsSuspensionForce * timeStep * wheelInfo.m_frictionSlip;
				btScalar maximpSide = 
					maximp;
				btScalar maximpSquared = 
					maximp * maximpSide;
			
				m_forwardImpulse[wheel] = rollingFriction;

				btScalar x = (m_forwardImpulse[wheel] ) * fwdFactor;
				btScalar y = (m_sideImpulse[wheel] ) * sideFactor;
				
				btScalar impulseSquared = (x*x + y*y);

				if (impulseSquared > maximpSquared)
				{
					sliding = true;
					btScalar factor = maximp / btSqrt(impulseSquared);
					m_wheelInfo[wheel].m_skidInfo *= factor;
				}
			} 

		}
	}

	
	// All the wheels with side impulse (those standing non-straight)
	// will get the skid information, too
	if (sliding)
	{
		for (int wheel = 0;wheel < getNumWheels(); wheel++)
		{
			if (std::abs(m_sideImpulse[wheel]) > magic_zero_constant)
			{
				if (m_wheelInfo[wheel].m_skidInfo < btScalar(1.))
				{
					m_forwardImpulse[wheel] *=	m_wheelInfo[wheel].m_skidInfo;
					m_sideImpulse[wheel] *= m_wheelInfo[wheel].m_skidInfo;
				}
			}
		}
	}

	// apply the impulses
	{
		for (int wheel = 0;wheel<getNumWheels() ; wheel++)
		{
			btWheelInfo& wheelInfo = m_wheelInfo[wheel];

			btVector3 rel_pos = 
				wheelInfo.m_raycastInfo.m_contactPointWS - 
				m_chassisBody->getCenterOfMassPosition();

			if (std::abs(m_forwardImpulse[wheel]) > magic_zero_constant)
				m_chassisBody->applyImpulse(
					m_forwardWS[wheel]*(m_forwardImpulse[wheel]),
					rel_pos);

			if (std::abs(m_sideImpulse[wheel]) > magic_zero_constant)
			{
				btRigidBody* groundObject = 
					static_cast<btRigidBody*>(
						m_wheelInfo[wheel].m_raycastInfo.m_groundObject);

				btVector3 rel_pos2 = 
					wheelInfo.m_raycastInfo.m_contactPointWS - 
					groundObject->getCenterOfMassPosition();

				btVector3 sideImp = m_axle[wheel] * m_sideImpulse[wheel];

				rel_pos[m_indexUpAxis] *= wheelInfo.m_rollInfluence;

				m_chassisBody->applyImpulse(
					sideImp,
					rel_pos);

				//apply friction impulse on the ground (useless, groundObject
				//is the fixed body)
				groundObject->applyImpulse(
					-sideImp,
					rel_pos2);
			}
		}
	}

	
}



void	myvehicle::debugDraw(btIDebugDraw* debugDrawer)
{

	for (int v=0;v<this->getNumWheels();v++)
	{
		btVector3 wheelColor(0,1,1);
		if (getWheelInfo(v).m_raycastInfo.m_isInContact)
		{
			wheelColor.setValue(0,0,1);
		} else
		{
			wheelColor.setValue(1,0,1);
		}

		btVector3 wheelPosWS = getWheelInfo(v).m_worldTransform.getOrigin();

		btVector3 axle = btVector3(	
			getWheelInfo(v).m_worldTransform.getBasis()[0][getRightAxis()],
			getWheelInfo(v).m_worldTransform.getBasis()[1][getRightAxis()],
			getWheelInfo(v).m_worldTransform.getBasis()[2][getRightAxis()]);

		//debug wheels (cylinders)
		debugDrawer->drawLine(wheelPosWS,wheelPosWS+axle,wheelColor);
		debugDrawer->drawLine(wheelPosWS,getWheelInfo(v).m_raycastInfo.m_contactPointWS,wheelColor);

	}
}


void* btDefaultVehicleRaycaster::castRay(const btVector3& from,const btVector3& to, btVehicleRaycasterResult& result)
{
//	RayResultCallback& resultCallback;

	btCollisionWorld::ClosestRayResultCallback rayCallback(from,to);

	m_dynamicsWorld->rayTest(from, to, rayCallback);

	if (rayCallback.hasHit())
	{
		
		btRigidBody* body = btRigidBody::upcast(rayCallback.m_collisionObject);
        if (body && body->hasContactResponse())
		{
			result.m_hitPointInWorld = rayCallback.m_hitPointWorld;
			result.m_hitNormalInWorld = rayCallback.m_hitNormalWorld;
			result.m_hitNormalInWorld.normalize();
			result.m_distFraction = rayCallback.m_closestHitFraction;
			return body;
		}
	}
	return 0;
}
