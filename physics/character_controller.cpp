#include "character_controller.hpp"
#include "transform_to_gizmo.hpp"
#include "vec3_to_bullet.hpp"
#include "world.hpp"
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <fcppt/math/vector/arithmetic.hpp>

insula::physics::character_controller::character_controller(
	world &_world,
	scalar width,
	scalar height,
	vec3 const &position)
:
	physics::object(
		object_type::character),
	world_(
		_world),
	shape_(
		width,
		height),
	ghost_object_(),
	walk_vector_(
		vec3::null())
{
	// Ghost init. begin
	ghost_object_.setUserPointer(
		this);
	ghost_object_.setWorldTransform(
		btTransform(
			btMatrix3x3::getIdentity(),
			// NOTE: We "correct" the player's position manually here. I
			// don't know if this is better done outside this class
			vec3_to_bullet(
				position + 
				physics::vec3(0,height/2,0))));
	ghost_object_.setCollisionShape(
		&shape_);
	// I don't know what this is for
	ghost_object_.setCollisionFlags(
		btCollisionObject::CF_CHARACTER_OBJECT);
	world_.handle().getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(
		new btGhostPairCallback());
	// Ghost init. end

	impl_.reset(
		new btKinematicCharacterController(
			&ghost_object_,
			&shape_,
			static_cast<scalar>(
				0.35),
			// up axis
			1));

	world_.handle().addCollisionObject(
		&ghost_object_,
		btBroadphaseProxy::CharacterFilter, 
		btBroadphaseProxy::StaticFilter
			| btBroadphaseProxy::DefaultFilter);
	world_.handle().addAction(
		impl_.get());
}

insula::physics::gizmo const
insula::physics::character_controller::gizmo() const
{
	return 
		transform_to_gizmo(
			ghost_object_.getWorldTransform());
}

void
insula::physics::character_controller::update(
	time_delta const delta)
{
	impl_->setWalkDirection(
		vec3_to_bullet(
			walk_vector_ * delta));
}

void
insula::physics::character_controller::walk_vector(
	vec3 const &_walk_vector)
{
	walk_vector_ = _walk_vector;
}

insula::physics::character_controller::~character_controller()
{
	world_.handle().removeCollisionObject(
		&ghost_object_);
	world_.handle().removeAction(
		impl_.get());
}
