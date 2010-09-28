#include "character_controller.hpp"
#include "transform_to_gizmo.hpp"
#include "vec3_to_bullet.hpp"

insula::physics::character_controller::character_controller(
	scalar width,
	scalar height,
	vec3 const &position)
:
	shape_(
		width,
		height),
	ghost_object_(),
	impl_(
		&ghost_object_,
		&shape_,
		static_cast<scalar>(
			0.35),
		// up axis
		1)
{
	ghost_object_.setWorldTransform(
		btTransform(
			btMatrix3x3::getIdentity(),
			vec3_to_bullet(
				position)));
	ghost_object_.setCollisionShape(
		&shape_);
	// I don't know what this is for
	ghost_object_.setCollisionFlags(
		btCollisionObject::CF_CHARACTER_OBJECT);
}

insula::physics::gizmo const
insula::physics::character_controller::gizmo() const
{
	return 
		transform_to_gizmo(
			ghost_object_.getWorldTransform());
}
