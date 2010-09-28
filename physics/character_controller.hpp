#ifndef INSULA_PHYSICS_CHARACTER_CONTROLLER_HPP_INCLUDED
#define INSULA_PHYSICS_CHARACTER_CONTROLLER_HPP_INCLUDED

#include "vec3.hpp"
#include "scalar.hpp"
#include "gizmo.hpp"
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

namespace insula
{
namespace physics
{
class character_controller
{
public:
	explicit
	character_controller(
		scalar width,
		scalar height,
		vec3 const &position);

	physics::gizmo const
	gizmo() const;
private:
	btCapsuleShape shape_;
	btPairCachingGhostObject ghost_object_;
	btKinematicCharacterController impl_;
};
}
}

#endif
