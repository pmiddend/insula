#ifndef INSULA_PHYSICS_VEHICLE_MYVEHICLE_HPP_INCLUDED
#define INSULA_PHYSICS_VEHICLE_MYVEHICLE_HPP_INCLUDED

#include "shared_raycaster_ptr.hpp"
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/ConstraintSolver/btTypedConstraint.h>
#include <BulletDynamics/Vehicle/btVehicleRaycaster.h>
#include <LinearMath/btAlignedObjectArray.h>
#include <BulletDynamics/Vehicle/btWheelInfo.h>
#include <BulletDynamics/Dynamics/btActionInterface.h>
#include <vector>

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
	btScalar	m_steeringValue; 
	btScalar m_currentVehicleSpeedKmHour;

	btRigidBody* m_chassisBody;

	int m_indexRightAxis;
	int m_indexUpAxis;
	int	m_indexForwardAxis;
public:
	explicit
	myvehicle(
		btRigidBody* chassis,	
		shared_raycaster_ptr raycaster);

	~myvehicle();

	void 
	updateAction(
		btCollisionWorld*, 
		btScalar step);
	

	///btActionInterface interface
	void	debugDraw(btIDebugDraw* debugDrawer);
			
	const btTransform& getChassisWorldTransform() const;
	
	btScalar rayCast(btWheelInfo& wheel);
	
	
	void resetSuspension();

	btScalar	getSteeringValue(int wheel) const;

	void	setSteeringValue(btScalar steering,int wheel);


	void	applyEngineForce(btScalar force, int wheel);

	const btTransform&	getWheelTransformWS( int wheelIndex ) const;

	void
	update_wheel_transform(
		btWheelInfo &, 
		bool interpolatedTransform = true);
	
	void	setRaycastWheelInfo( int wheelIndex , bool isInContact, const btVector3& hitPoint, const btVector3& hitNormal,btScalar depth);

	btWheelInfo&	addWheel( const btVector3& connectionPointCS0, const btVector3& wheelDirectionCS0,const btVector3& wheelAxleCS,btScalar suspensionRestLength,btScalar wheelRadius,const btVehicleTuning& tuning, bool isFrontWheel);

	inline int		getNumWheels() const {
		return int (m_wheelInfo.size());
	}
	
	typedef
	std::vector
	<
		btWheelInfo
	>
	wheel_info_sequence;

	wheel_info_sequence m_wheelInfo;
	//btAlignedObjectArray<btWheelInfo>	m_wheelInfo;


	const btWheelInfo&	getWheelInfo(int index) const;

	btWheelInfo&	getWheelInfo(int index);

	void	updateWheelTransformsWS(btWheelInfo& wheel , bool interpolatedTransform = true);

	
	void setBrake(btScalar brake,int wheelIndex);

	void	
	update_friction(
		btScalar);



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
private:
	void	
	update_suspension(
		btScalar deltaTime);

	void
	update_speed();

	btScalar
	calculate_suspension_force(
		btWheelInfo const &);

public:
	// DEBUG
	bool
	wheel_on_ground(
		unsigned);
};
}
}
}


#endif //RAYCASTVEHICLE_H

