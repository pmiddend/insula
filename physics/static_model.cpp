#include "static_model.hpp"
#include "dim3_to_bullet.hpp"
#include "vec3_to_bullet.hpp"
#include "transform_to_mat4.hpp"
#include "dim3.hpp"
#include "scalar.hpp"
#include "../model/object.hpp"
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <LinearMath/btMotionState.h>
#include <LinearMath/btMatrix3x3.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btDefaultMotionState.h>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/dim/arithmetic.hpp>

insula::physics::static_model::static_model(
	world &w,
	vec3 const &position,
	model::object_ptr const _model,
	model_approximation const &_approx,
	solidity::type const _solidity)
:
	model_(
		_model),
	body_(),
	motion_state_(
		new btDefaultMotionState(
			btTransform(
				btMatrix3x3::getIdentity(),
				vec3_to_bullet(
					position)))),
	shape_(),
	world_scope_(
		w)
{
	switch (_approx.t)
	{
		case model_approximation::exact:
			
		break;
		case model_approximation::box:
			shape_.reset(
				new btBoxShape(
					dim3_to_bullet(
						// btBoxShape gets half extents, so muliply by 0.5 here
						static_cast<scalar>(0.5)*
						fcppt::math::dim::structure_cast<dim3>(
							model_->bounding_box().dimension()))));
		break;
		case model_approximation::sphere:
			shape_.reset(
				new btSphereShape(
					length(
						static_cast<scalar>(0.5) * 
						fcppt::math::dim::structure_cast<vec3>(
							model_->bounding_box().dimension()))));
		break;
		case model_approximation::cylinder_x:

		break;
		case model_approximation::cylinder_y:

		break;
		case model_approximation::cylinder_z:

		break;
	}

	body_.reset(
		new btRigidBody(
			btRigidBody::btRigidBodyConstructionInfo(
				static_cast<btScalar>(0),
				motion_state_.get(),
				shape_.get(),
				btVector3(0,0,0))));

	world_scope_.set(
		*body_);

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

void
insula::physics::static_model::render()
{
	btTransform t;
	motion_state_->getWorldTransform(
		t);
	model_->render(
		transform_to_mat4(
			t));
}

insula::physics::static_model::~static_model()
{
}
