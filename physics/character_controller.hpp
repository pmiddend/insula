#ifndef INSULA_PHYSICS_CHARACTER_CONTROLLER_HPP_INCLUDED
#define INSULA_PHYSICS_CHARACTER_CONTROLLER_HPP_INCLUDED

#include "vec3.hpp"
#include "scalar.hpp"
#include "gizmo.hpp"
#include "object.hpp"
#include "world_fwd.hpp"
#include "../time_delta.hpp"
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Dynamics/btActionInterface.h>
#include <memory>

namespace insula
{
namespace physics
{
class character_controller
:
	public object,
	public btActionInterface
{
public:
	explicit
	character_controller(
		world &,
		scalar width,
		scalar height,
		vec3 const &position);

	physics::gizmo const
	gizmo() const;

	void
	update(
		time_delta);

	void
	walk_vector(
		vec3 const &);

	// @override
	void
	updateAction(
		btCollisionWorld *,
		btScalar);

	// @override
	void
	debugDraw(
		btIDebugDraw *);

	~character_controller();
private:
	world &world_;
	// The shape cannot be const, since we modify it temporarily
	btCapsuleShape shape_;
	btPairCachingGhostObject ghost_object_;
	btVector3 walk_vector_;
	btVector3 normalized_walk_vector_;

	// This is set to true if the character touched/penetrated some
	// object
	bool touching_contact_;
	btVector3 current_position_,target_position_;
	btVector3 touching_normal_;
	scalar step_height_;
	scalar current_step_offset_;

	bool
	recover_from_penetration(
		btCollisionWorld &);

	void
	step_up(
		btCollisionWorld &);

	void
	step_forward(
		btCollisionWorld &,
		btScalar);

	void
	step_down(
		btCollisionWorld &);
	
	void
	update_target_position(
		btVector3 const &);
};
}
}

#endif
