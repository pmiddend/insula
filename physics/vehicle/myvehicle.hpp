#ifndef INSULA_PHYSICS_VEHICLE_MYVEHICLE_HPP_INCLUDED
#define INSULA_PHYSICS_VEHICLE_MYVEHICLE_HPP_INCLUDED

#include "shared_raycaster_ptr.hpp"
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/ConstraintSolver/btTypedConstraint.h>
#include <BulletDynamics/Vehicle/btVehicleRaycaster.h>
#include <LinearMath/btAlignedObjectArray.h>
#include <BulletDynamics/Vehicle/btWheelInfo.h>
#include <BulletDynamics/Dynamics/btActionInterface.h>

class btVehicleTuning;
class btDynamicsWorld;

namespace insula
{
namespace physics
{
namespace vehicle
{
class myvehicle 
: 
	public btActionInterface
{
		btAlignedObjectArray<btVector3>	m_forwardWS;
		btAlignedObjectArray<btVector3>	m_axle;
		btAlignedObjectArray<btScalar>	m_forwardImpulse;
		btAlignedObjectArray<btScalar>	m_sideImpulse;
	
		///backwards compatibility
		int	m_userConstraintType;
		int	m_userConstraintId;

public:
	class btVehicleTuning
		{
			public:

			btVehicleTuning()
				:m_suspensionStiffness(btScalar(5.88)),
				m_suspensionCompression(btScalar(0.83)),
				m_suspensionDamping(btScalar(0.88)),
				m_maxSuspensionTravelCm(btScalar(500.)),
				m_frictionSlip(btScalar(10.5)),
				m_maxSuspensionForce(btScalar(6000.))
			{
			}
			btScalar	m_suspensionStiffness;
			btScalar	m_suspensionCompression;
			btScalar	m_suspensionDamping;
			btScalar	m_maxSuspensionTravelCm;
			btScalar	m_frictionSlip;
			btScalar	m_maxSuspensionForce;

		};
private:

	btScalar	m_tau;
	btScalar	m_damping;
	shared_raycaster_ptr	m_vehicleRaycaster;
	btScalar		m_pitchControl;
	btScalar	m_steeringValue; 
	btScalar m_currentVehicleSpeedKmHour;

	btRigidBody* m_chassisBody;

	int m_indexRightAxis;
	int m_indexUpAxis;
	int	m_indexForwardAxis;

	void defaultInit(const btVehicleTuning& tuning);

public:

	//constructor to create a car from an existing rigidbody
	myvehicle(
		btVehicleTuning const & tuning,
		btRigidBody* chassis,	
		shared_raycaster_ptr raycaster );

	virtual ~myvehicle() ;


	///btActionInterface interface
	virtual void updateAction( btCollisionWorld* collisionWorld, btScalar step)
	{
        (void) collisionWorld;
		updateVehicle(step);
	}
	

	///btActionInterface interface
	void	debugDraw(btIDebugDraw* debugDrawer);
			
	const btTransform& getChassisWorldTransform() const;
	
	btScalar rayCast(btWheelInfo& wheel);

	virtual void updateVehicle(btScalar step);
	
	
	void resetSuspension();

	btScalar	getSteeringValue(int wheel) const;

	void	setSteeringValue(btScalar steering,int wheel);


	void	applyEngineForce(btScalar force, int wheel);

	const btTransform&	getWheelTransformWS( int wheelIndex ) const;

	void	updateWheelTransform( int wheelIndex, bool interpolatedTransform = true );
	
	void	setRaycastWheelInfo( int wheelIndex , bool isInContact, const btVector3& hitPoint, const btVector3& hitNormal,btScalar depth);

	btWheelInfo&	addWheel( const btVector3& connectionPointCS0, const btVector3& wheelDirectionCS0,const btVector3& wheelAxleCS,btScalar suspensionRestLength,btScalar wheelRadius,const btVehicleTuning& tuning, bool isFrontWheel);

	inline int		getNumWheels() const {
		return int (m_wheelInfo.size());
	}
	
	btAlignedObjectArray<btWheelInfo>	m_wheelInfo;


	const btWheelInfo&	getWheelInfo(int index) const;

	btWheelInfo&	getWheelInfo(int index);

	void	updateWheelTransformsWS(btWheelInfo& wheel , bool interpolatedTransform = true);

	
	void setBrake(btScalar brake,int wheelIndex);

	void	setPitchControl(btScalar pitch)
	{
		m_pitchControl = pitch;
	}
	
	void	updateSuspension(btScalar deltaTime);

	virtual void	updateFriction(btScalar	timeStep);



	inline btRigidBody* getRigidBody()
	{
		return m_chassisBody;
	}

	const btRigidBody* getRigidBody() const
	{
		return m_chassisBody;
	}

	inline int	getRightAxis() const
	{
		return m_indexRightAxis;
	}
	inline int getUpAxis() const
	{
		return m_indexUpAxis;
	}

	inline int getForwardAxis() const
	{
		return m_indexForwardAxis;
	}

	
	///Worldspace forward vector
	btVector3 getForwardVector() const
	{
		const btTransform& chassisTrans = getChassisWorldTransform(); 

		btVector3 forwardW ( 
			  chassisTrans.getBasis()[0][m_indexForwardAxis], 
			  chassisTrans.getBasis()[1][m_indexForwardAxis], 
			  chassisTrans.getBasis()[2][m_indexForwardAxis]); 

		return forwardW;
	}

	///Velocity of vehicle (positive if velocity vector has same direction as foward vector)
	btScalar	getCurrentSpeedKmHour() const
	{
		return m_currentVehicleSpeedKmHour;
	}

	virtual void	setCoordinateSystem(int rightIndex,int upIndex,int forwardIndex)
	{
		m_indexRightAxis = rightIndex;
		m_indexUpAxis = upIndex;
		m_indexForwardAxis = forwardIndex;
	}


	///backwards compatibility
	int getUserConstraintType() const
	{
		return m_userConstraintType ;
	}

	void	setUserConstraintType(int userConstraintType)
	{
		m_userConstraintType = userConstraintType;
	};

	void	setUserConstraintId(int uid)
	{
		m_userConstraintId = uid;
	}

	int getUserConstraintId() const
	{
		return m_userConstraintId;
	}

};
}
}
}

class btDefaultVehicleRaycaster : public btVehicleRaycaster
{
	btDynamicsWorld*	m_dynamicsWorld;
public:
	btDefaultVehicleRaycaster(btDynamicsWorld* world)
		:m_dynamicsWorld(world)
	{
	}

	virtual void* castRay(const btVector3& from,const btVector3& to, btVehicleRaycasterResult& result);

};


#endif //RAYCASTVEHICLE_H

