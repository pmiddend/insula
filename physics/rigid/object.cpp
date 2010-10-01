#include "object.hpp"
#include "parameters.hpp"
#include "../dim3_to_bullet.hpp"
#include "../transform_to_mat4.hpp"
#include "../bullet_to_vec3.hpp"
#include "../vec3_to_bullet.hpp"
#include "../dim3.hpp"
#include "../scalar.hpp"
#include "../motion_state.hpp"
#include "../../model/object.hpp"
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <fcppt/io/cout.hpp>
#include <iostream>

namespace
{
btRigidBody::btRigidBodyConstructionInfo const
create_construction_info(
	btMotionState *motion_state,
	btCollisionShape *shape,
	fcppt::optional<insula::physics::scalar> const &mass)
{
	btVector3 inertia(0,0,0);
	if (mass)
		shape->calculateLocalInertia(
			*mass,
			inertia);
	return 
		btRigidBody::btRigidBodyConstructionInfo(
				mass 
				? 
					*mass 
				: 
					static_cast<btScalar>(0),
				motion_state,
				shape,
				inertia);
}
}

insula::physics::rigid::object::object(
	parameters const &params)
:
	physics::object(
		params.type),
	motion_state_(
		new motion_state(
			params.position,
			params.transformation)),
	shape_(
		params.shape),
	body_(
		new btRigidBody(
			create_construction_info(
				motion_state_.get(),
				shape_.get(),
				params.mass)))
{
	// It's important that we _first_ set the user pointer and _then_
	// add the body because the constructor (indirectly) calls the
	// collision filter which in turn wants the correct user pointer
	body_->setUserPointer(
		this);

	world_scope_.reset(
		new scoped_body(
			params.world_,
			*body_));

	switch (params.solidity)
	{
		case solidity::solid:
			break;
		case solidity::nonsolid:
			body_->setCollisionFlags(
				body_->getCollisionFlags() | 
				btCollisionObject::CF_NO_CONTACT_RESPONSE);
			break;
	}

	body_->setLinearVelocity(
		vec3_to_bullet(
			params.linear_velocity));
}

insula::physics::vec3 const
insula::physics::rigid::object::position() const
{
	return 
		bullet_to_vec3(
			motion_state_->transform().getOrigin());
}

insula::physics::mat4 const
insula::physics::rigid::object::world_transform() const
{
	return 
		transform_to_mat4(
			motion_state_->transform());
}

insula::physics::rigid::object::~object()
{
}
