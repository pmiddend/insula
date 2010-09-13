#include "static_model.hpp"
#include "dim3_to_bullet.hpp"
#include "transform_to_mat4.hpp"
#include "dim3.hpp"
#include "scalar.hpp"
#include "motion_state.hpp"
#include "static_model_parameters.hpp"
#include "../model/object.hpp"
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <fcppt/io/cout.hpp>
#include <iostream>

insula::physics::static_model::static_model(
	static_model_parameters const &params)
:
	object(
		params.type),
	motion_state_(
		new motion_state(
			params.position,
			params.transformation)),
	shape_(
		params.shape),
	body_(
		new btRigidBody(
			btRigidBody::btRigidBodyConstructionInfo(
				static_cast<btScalar>(0),
				motion_state_.get(),
				shape_.get(),
				btVector3(0,0,0))))
{
	// It's important that we _first_ set the user pointer and _then_
	// add the body because the constructor (indirectly) calls the
	// collision filter which in turn wants the correct user pointer
	body_->setUserPointer(
		this);


	world_scope_.reset(
		new scoped_body(params.world_,*body_));

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
