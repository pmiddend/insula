#include "ghost.hpp"
#include "mat3_to_bullet.hpp"
#include "vec3_to_bullet.hpp"
#include "scalar.hpp"
#include "ghost_parameters.hpp"
#include "world.hpp"
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <LinearMath/btTransform.h>

insula::physics::ghost::ghost(
	ghost_parameters const &params)
:
	object(
		object_type::ghost),
	world_(
		params.world_.handle())
{
	m_internalType = CO_COLLISION_OBJECT;
	m_collisionFlags |= CF_STATIC_OBJECT;
	m_worldTransform = 
		btTransform(
			mat3_to_bullet(
				params.transformation),
			vec3_to_bullet(
				params.position));
	setCollisionShape(
		params.shape.get());
	m_interpolationWorldTransform = m_worldTransform;
	m_interpolationLinearVelocity.setValue(0,0,0);
	m_interpolationAngularVelocity.setValue(0,0,0);
	m_friction = static_cast<scalar>(0);
	m_restitution = static_cast<scalar>(0);
	m_userObjectPointer = this;

	world_.addCollisionObject(
		this);
}

insula::physics::ghost::~ghost()
{
	world_.removeCollisionObject(
		this);
}
