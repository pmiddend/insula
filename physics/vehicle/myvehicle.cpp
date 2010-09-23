#include "myvehicle.hpp"
#include "raycaster.hpp"
#include "../bullet_to_vec3.hpp"
#include "../vec3_to_bullet.hpp"

// bullet stuff begin
#include <BulletDynamics/ConstraintSolver/btSolve2LinearConstraint.h>
#include <LinearMath/btVector3.h>
#include <BulletDynamics/ConstraintSolver/btJacobianEntry.h>
#include <LinearMath/btQuaternion.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletDynamics/Vehicle/btVehicleRaycaster.h>
#include <BulletDynamics/Vehicle/btWheelInfo.h>
#include <LinearMath/btMinMax.h>
#include <LinearMath/btIDebugDraw.h>
#include <BulletDynamics/ConstraintSolver/btContactConstraint.h>
// bullet stuff end

#include <boost/foreach.hpp>
#include <fcppt/math/clamp.hpp>
#include <cmath>
#include <iostream>


namespace
{
bool
is_zero(btScalar const s)
{
	btScalar const magic_zero_constant = static_cast<btScalar>(0.0001);
	return std::abs(s) < magic_zero_constant;
}
}

insula::physics::vehicle::myvehicle::myvehicle(
	btRigidBody* chassis,	
	shared_raycaster_ptr raycaster )
:
	m_vehicleRaycaster(
		raycaster)
{
	m_chassisBody = chassis;
	m_indexRightAxis = 0;
	m_indexUpAxis = 2;
	m_indexForwardAxis = 1;
	m_currentVehicleSpeedKmHour = btScalar(0.);
	m_steeringValue = btScalar(0.);
}

insula::physics::vehicle::myvehicle::~myvehicle()
{
}


//
// basically most of the code is general for 2 or 4 wheel vehicles, but some of it needs to be reviewed
//
btWheelInfo&	
insula::physics::vehicle::myvehicle::myvehicle::addWheel( const btVector3& connectionPointCS, const btVector3& wheelDirectionCS0,const btVector3& wheelAxleCS, btScalar suspensionRestLength, btScalar wheelRadius,const btVehicleTuning& tuning, bool isFrontWheel)
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
	update_wheel_transform(
		m_wheelInfo.back(),
		false);
	return wheel;
}




const btTransform&	
insula::physics::vehicle::myvehicle::myvehicle::getWheelTransformWS( int wheelIndex ) const
{
	btAssert(wheelIndex < getNumWheels());
	const btWheelInfo& wheel = m_wheelInfo[wheelIndex];
	return wheel.m_worldTransform;

}

void	
insula::physics::vehicle::myvehicle::myvehicle::update_wheel_transform(
	btWheelInfo &wheel, 
	bool const interpolatedTransform)
{
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

void 
insula::physics::vehicle::myvehicle::myvehicle::resetSuspension()
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

void	
insula::physics::vehicle::myvehicle::myvehicle::updateWheelTransformsWS(btWheelInfo& wheel , bool interpolatedTransform)
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

btScalar 
insula::physics::vehicle::myvehicle::myvehicle::rayCast(
	btWheelInfo &wheel)
{
	updateWheelTransformsWS( 
		wheel,
		false);

	// Penetration depth
	btScalar depth = 
		static_cast<btScalar>(-1);
	
	// Shoot the ray "too far" into the ground (by the suspension rest
	// length) and later compensate that by compressing the suspension
	// string.
	btScalar const raylen = 
		wheel.getSuspensionRestLength()+
		wheel.m_wheelsRadius;

	btVector3 const 
		rayvector = 
			wheel.m_raycastInfo.m_wheelDirectionWS * raylen,
		source = 
			wheel.m_raycastInfo.m_hardPointWS,
		target = 
			source + rayvector;

	// Preliminary contact point is the target
	wheel.m_raycastInfo.m_contactPointWS = 
		target;

	raycast_results const ray_results = 
		m_vehicleRaycaster->cast_ray(
			source,
			target);

	wheel.m_raycastInfo.m_groundObject = 
		0;

	if (ray_results.object)
	{
		wheel.m_raycastInfo.m_isInContact = 
			true;
		depth = 
			raylen * ray_results.dist_fraction;
		wheel.m_raycastInfo.m_contactNormalWS  = 
			ray_results.normal;
		wheel.m_raycastInfo.m_groundObject = 
			&getFixedBody();

		btScalar const hitDistance = 
			ray_results.dist_fraction * raylen;
		wheel.m_raycastInfo.m_suspensionLength = 
			hitDistance - wheel.m_wheelsRadius;

		wheel.m_raycastInfo.m_suspensionLength = 
			fcppt::math::clamp(
				wheel.m_raycastInfo.m_suspensionLength,
				wheel.getSuspensionRestLength() - 
					wheel.m_maxSuspensionTravelCm * static_cast<btScalar>(0.01),
				wheel.getSuspensionRestLength() + 
					wheel.m_maxSuspensionTravelCm * static_cast<btScalar>(0.01));

		wheel.m_raycastInfo.m_contactPointWS = 
			ray_results.point;

		btScalar const denominator = 
			wheel.m_raycastInfo.m_contactNormalWS.dot(
				wheel.m_raycastInfo.m_wheelDirectionWS);

		btVector3 const 
			chassis_velocity_at_contact_point = 
				getRigidBody()->getVelocityInLocalPoint(
					wheel.m_raycastInfo.m_contactPointWS-getRigidBody()->getCenterOfMassPosition());

		// The chassis velocity and the contact normal should normally be
		// orthogonal, so this is zero
		btScalar const projVel = 
			wheel.m_raycastInfo.m_contactNormalWS.dot( 
				chassis_velocity_at_contact_point);

		if (denominator >= btScalar(-0.1))
		{
			wheel.m_suspensionRelativeVelocity = 
				static_cast<btScalar>(0);
			wheel.m_clippedInvContactDotSuspension = 
				static_cast<btScalar>(1) / static_cast<btScalar>(0.1);
		}
		else
		{
			btScalar const inv = 
				static_cast<btScalar>(-1) / denominator;
			wheel.m_suspensionRelativeVelocity = 
				projVel * inv;
			wheel.m_clippedInvContactDotSuspension = 
				inv;
		}
			
	} 
	else
	{
		// Put wheel info as in rest position
		wheel.m_raycastInfo.m_suspensionLength = 
			wheel.getSuspensionRestLength();
		wheel.m_suspensionRelativeVelocity = 
			static_cast<btScalar>(0);
		wheel.m_raycastInfo.m_contactNormalWS = 
			-wheel.m_raycastInfo.m_wheelDirectionWS;
		wheel.m_clippedInvContactDotSuspension = 
			static_cast<btScalar>(1);
	}

	return depth;
}


const btTransform& insula::physics::vehicle::myvehicle::myvehicle::getChassisWorldTransform() const
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


void 
insula::physics::vehicle::myvehicle::myvehicle::updateAction(
	btCollisionWorld *,
	btScalar const step)
{
	update_speed();

	BOOST_FOREACH(
		wheel_info_sequence::reference wheel,
		m_wheelInfo)
	{
		update_wheel_transform(
			wheel,
			false);
		
		rayCast(
			wheel);

		wheel.m_wheelsSuspensionForce = 
			calculate_suspension_force(
				wheel);
		
		// Apply an impulse in the direction of the normal ("upwards", so
		// to speak) at the contact point of the wheel, physically accurate
		getRigidBody()->applyImpulse(
			wheel.m_raycastInfo.m_contactNormalWS 
				* wheel.m_wheelsSuspensionForce 
				* step, 
			wheel.m_raycastInfo.m_contactPointWS 
				- getRigidBody()->getCenterOfMassPosition());
	
	}
	
	update_friction( step);

	
	// Updates the wheel rotation (not important)
	for (int i=0;i<m_wheelInfo.size();i++)
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


void	insula::physics::vehicle::myvehicle::myvehicle::setSteeringValue(btScalar steering,int wheel)
{
	btAssert(wheel>=0 && wheel < getNumWheels());

	btWheelInfo& wheelInfo = getWheelInfo(wheel);
	wheelInfo.m_steering = steering;
}



btScalar	insula::physics::vehicle::myvehicle::myvehicle::getSteeringValue(int wheel) const
{
	return getWheelInfo(wheel).m_steering;
}


void	insula::physics::vehicle::myvehicle::myvehicle::applyEngineForce(btScalar force, int wheel)
{
	btAssert(wheel>=0 && wheel < getNumWheels());
	btWheelInfo& wheelInfo = getWheelInfo(wheel);
	wheelInfo.m_engineForce = force;
}


const btWheelInfo&	insula::physics::vehicle::myvehicle::myvehicle::getWheelInfo(int index) const
{
	btAssert((index >= 0) && (index < 	getNumWheels()));
	
	return m_wheelInfo[index];
}

btWheelInfo&	insula::physics::vehicle::myvehicle::myvehicle::getWheelInfo(int index) 
{
	btAssert((index >= 0) && (index < 	getNumWheels()));
	
	return m_wheelInfo[index];
}

void insula::physics::vehicle::myvehicle::myvehicle::setBrake(btScalar brake,int wheelIndex)
{
	btAssert((wheelIndex >= 0) && (wheelIndex < 	getNumWheels()));
	getWheelInfo(wheelIndex).m_brake = brake;
}


// This function calculates the suspension force for each . That's the force
// that pushes the wheels down on the ground. It's of course
// proportional to the mass, but also to the compression of the
// suspension spring.
void	
insula::physics::vehicle::myvehicle::myvehicle::update_suspension(
	btScalar /*time_step*/)
{
	BOOST_FOREACH(
		wheel_info_sequence::reference wheel_info,
		m_wheelInfo)
		wheel_info.m_wheelsSuspensionForce = 
			calculate_suspension_force(
				wheel_info);
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




void	
insula::physics::vehicle::myvehicle::myvehicle::update_friction(
	btScalar const timeStep)
{
	if (m_wheelInfo.empty())
		return;

	typedef
	std::vector
	<
		btScalar
	>
	impulse_array;

	typedef
	std::vector
	<
		btVector3
	>
	vector_array;

	impulse_array 
		m_forwardImpulse(
			m_wheelInfo.size()),
		m_sideImpulse(
			m_wheelInfo.size());

	vector_array 
		m_forwardWS(
			m_wheelInfo.size()),
		m_axle(
			m_wheelInfo.size());
		
	for (wheel_info_sequence::size_type i = 0; i < m_wheelInfo.size(); i++)
	{
		btWheelInfo &wheelInfo = m_wheelInfo[i];
					
		btRigidBody *groundObject = 
			static_cast<btRigidBody *>(
				wheelInfo.m_raycastInfo.m_groundObject);

		if (!groundObject)
			continue;

		btTransform const & wheelTrans = 
			wheelInfo.m_worldTransform;

		btMatrix3x3 const wheelBasis0 = 
			wheelTrans.getBasis();

		m_axle[i] = btVector3(	
			wheelBasis0[0][m_indexRightAxis],
			wheelBasis0[1][m_indexRightAxis],
			wheelBasis0[2][m_indexRightAxis]);
					
		// This calculates the vector which points from the tip of the
		// normal vector "downwards" on the wheel axis
		//         ^
    //        /
    //       / |
    //    n /  |  result
    //     /   |   
    //    /    v      
		//		------------>
		//          axle
		btVector3 const &surfNormalWS = 
			wheelInfo.m_raycastInfo.m_contactNormalWS;
		btScalar const proj = 
			m_axle[i].dot(
				surfNormalWS);
		m_axle[i] -= surfNormalWS * proj;
		m_axle[i] = m_axle[i].normalize();
					
		m_forwardWS[i] = 
			surfNormalWS.cross(
				m_axle[i]).normalize();

		// This takes both bodies' velocities and calculates the
		// magnitude of the side impulse
		resolveSingleBilateral(
			*m_chassisBody, 
			wheelInfo.m_raycastInfo.m_contactPointWS,
			*groundObject, wheelInfo.m_raycastInfo.m_contactPointWS,
			// Distance
			static_cast<btScalar>(0),
			// Normal (which is the axle, because it points down, see above, WTF?)
			m_axle[i],
			// This is the "out" variable, all the other variables are untouched
			m_sideImpulse[i],
			timeStep);
	}

	// The sliding constant is global. One wheel slides => sliding = true
	bool sliding = false;
	// This loop is for rolling friction. 
	// If the vehicle is braking, this will calculate a negative force,
	// if it's accelerating, this will be a positive force,
	// if neither is the case, this will be a negative force (of with
	// later user-controllable magnitude)
	for (wheel_info_sequence::size_type wheel = 0; wheel < m_wheelInfo.size(); wheel++)
	{
		btWheelInfo &wheelInfo = m_wheelInfo[wheel];
		btRigidBody *groundObject = 
			static_cast<btRigidBody *>(
				wheelInfo.m_raycastInfo.m_groundObject);

		btScalar rollingFriction = 
			static_cast<btScalar>(0);

		if (groundObject)
		{
			if (!is_zero(wheelInfo.m_engineForce))
			{
				// Set the rolling friction (the force that resists the
				// wheel going forward) to the engine force (making it an
				// amplifying force)
				rollingFriction = 
					wheelInfo.m_engineForce * timeStep;
			} 
			else
			{
				// FIXME: Make this a vehicle parameter (this is rolling friction!)
				btScalar const defaultRollingFrictionImpulse = 
					static_cast<btScalar>(
						1);

				btScalar const maxImpulse = 
					!is_zero(wheelInfo.m_brake) 
					? 
						wheelInfo.m_brake 
					: 
						defaultRollingFrictionImpulse;

				btWheelContactPoint contactPt(
					// First body
					m_chassisBody,
					// Second body
					groundObject,
					// Friction Position World
					wheelInfo.m_raycastInfo.m_contactPointWS,
					// Friction Direction World
					m_forwardWS[wheel],
					// Maximum impulse
					maxImpulse);

				// I think this is the forward friction constraint
				rollingFriction = 
					calcRollingFriction(
						contactPt);
			}
		}

		m_forwardImpulse[wheel] = 
			static_cast<btScalar>(
				0);
		m_wheelInfo[wheel].m_skidInfo = 
			static_cast<btScalar>(
				1);

		if (groundObject)
		{
			m_wheelInfo[wheel].m_skidInfo= btScalar(1.);

			btScalar maximp = wheelInfo.m_wheelsSuspensionForce * timeStep * wheelInfo.m_frictionSlip;
			btScalar maximpSide = maximp;

			btScalar maximpSquared = maximp * maximpSide;


			m_forwardImpulse[wheel] = rollingFriction;//wheelInfo.m_engineForce* timeStep;
		// Why this?
		btScalar const 
			sideFactor = 
				static_cast<btScalar>(1),
			fwdFactor = 
				static_cast<btScalar>(0.5);


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

	


		if (sliding)
		{
			for (int wheel = 0;wheel < getNumWheels(); wheel++)
			{
				if (m_sideImpulse[wheel] != btScalar(0.))
				{
					if (m_wheelInfo[wheel].m_skidInfo< btScalar(1.))
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

				btVector3 rel_pos = wheelInfo.m_raycastInfo.m_contactPointWS - 
						m_chassisBody->getCenterOfMassPosition();

				if (m_forwardImpulse[wheel] != btScalar(0.))
				{
					m_chassisBody->applyImpulse(m_forwardWS[wheel]*(m_forwardImpulse[wheel]),rel_pos);
				}
				if (m_sideImpulse[wheel] != btScalar(0.))
				{
					class btRigidBody* groundObject = (class btRigidBody*) m_wheelInfo[wheel].m_raycastInfo.m_groundObject;

					btVector3 rel_pos2 = wheelInfo.m_raycastInfo.m_contactPointWS - 
						groundObject->getCenterOfMassPosition();

					
					btVector3 sideImp = m_axle[wheel] * m_sideImpulse[wheel];

					rel_pos[m_indexUpAxis] *= wheelInfo.m_rollInfluence;
					m_chassisBody->applyImpulse(sideImp,rel_pos);

					//apply friction impulse on the ground
					groundObject->applyImpulse(-sideImp,rel_pos2);
				}
			}
		}

	
}



void	
insula::physics::vehicle::myvehicle::myvehicle::debugDraw(
	btIDebugDraw* debugDrawer)
{
	for (int v=0;v<this->getNumWheels();v++)
	{
		btVector3 wheelColor(0,1,1);
		if (getWheelInfo(v).m_raycastInfo.m_groundObject)
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

void
insula::physics::vehicle::myvehicle::update_speed()
{
	m_currentVehicleSpeedKmHour = 
		btScalar(3.6) * 
		getRigidBody()->getLinearVelocity().dot(getForwardVector());
	
	btTransform const & chassisTrans = getChassisWorldTransform();

	btVector3 forwardW (
		chassisTrans.getBasis()[0][m_indexForwardAxis],
		chassisTrans.getBasis()[1][m_indexForwardAxis],
		chassisTrans.getBasis()[2][m_indexForwardAxis]);

	if (forwardW.dot(getRigidBody()->getLinearVelocity()) < btScalar(0.))
		m_currentVehicleSpeedKmHour *= btScalar(-1.);

}

btScalar
insula::physics::vehicle::myvehicle::calculate_suspension_force(
	btWheelInfo const &wheel_info)
{
	btScalar const chassisMass = 
		static_cast<btScalar>(1) / 
		m_chassisBody->getInvMass();

	// Technically speaking, a wheel which is not in contact with the
	// ground still has a force pushing down (the weight of the tire),
	// but we simplify here
	if (!wheel_info.m_raycastInfo.m_isInContact)
	{
		return 
			static_cast<btScalar>(0);
	}

	btScalar force;

	//	Spring
	{
		btScalar const 
			susp_length = 
				wheel_info.getSuspensionRestLength(),
			current_length = 
				wheel_info.m_raycastInfo.m_suspensionLength,
			length_diff = 
				susp_length - current_length;

		// Hooke's law!
		force = 
			wheel_info.m_suspensionStiffness * 
			length_diff * 
			wheel_info.m_clippedInvContactDotSuspension;
	}

	// Dampening of the spring
	{
		btScalar const projected_rel_vel = 
			wheel_info.m_suspensionRelativeVelocity;

		btScalar susp_damping;
		if (projected_rel_vel < static_cast<btScalar>(0))
			susp_damping = wheel_info.m_wheelsDampingCompression;
		else
			susp_damping = wheel_info.m_wheelsDampingRelaxation;
		force -= susp_damping * projected_rel_vel;
	}

	return 
		fcppt::math::clamp(
			force * chassisMass,
			static_cast<btScalar>(0),
			wheel_info.m_maxSuspensionForce);
}

bool
insula::physics::vehicle::myvehicle::wheel_on_ground(
	unsigned i)
{
	return m_wheelInfo[i].m_raycastInfo.m_groundObject;
}
