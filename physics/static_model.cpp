#include "static_model.hpp"
#include "dim3_to_bullet.hpp"
#include "transform_to_mat4.hpp"
#include "dim3.hpp"
#include "scalar.hpp"
#include "motion_state.hpp"
#include "../model/object.hpp"
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <fcppt/io/cout.hpp>
#include <iostream>

insula::physics::static_model::static_model(
	world &w,
	vec3 const &position,
	mat3 const &transformation,
	shared_shape_ptr const _shape,
	solidity::type const _solidity)
:
	body_(),
	motion_state_(
		new motion_state(
			position,
			transformation)),
	shape_(
		_shape)
{
	body_.reset(
		new btRigidBody(
			btRigidBody::btRigidBodyConstructionInfo(
				static_cast<btScalar>(0),
				motion_state_.get(),
				shape_.get(),
				btVector3(0,0,0))));

	world_scope_.reset(
		new scoped_body(w,*body_));

	switch (_solidity)
	{
		case solidity::solid:
			break;
		case solidity::nonsolid:
			body_->setCollisionFlags(
				body_->getCollisionFlags() | 
				btCollisionObject::CF_NO_CONTACT_RESPONSE);
			break;
	}

	body_->setUserPointer(
		this);

}

insula::physics::mat4 const
insula::physics::static_model::world_transform() const
{
	return 
		transform_to_mat4(
			motion_state_->transform());
}

insula::physics::static_model::~static_model()
{
}
