#ifndef INSULA_PHYSICS_CHARACTER_CONTROLLER_HPP_INCLUDED
#define INSULA_PHYSICS_CHARACTER_CONTROLLER_HPP_INCLUDED

#include "vec3.hpp"
#include "scalar.hpp"
#include "gizmo.hpp"
#include "object.hpp"
#include "world_fwd.hpp"
#include "../time_delta.hpp"
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <memory>

namespace insula
{
namespace physics
{
class character_controller
:
	public object
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

	~character_controller();
private:
	world &world_;
	btCapsuleShape shape_;
	btPairCachingGhostObject ghost_object_;
	std::unique_ptr<btKinematicCharacterController> impl_;
	vec3 walk_vector_;
};
}
}

#endif
