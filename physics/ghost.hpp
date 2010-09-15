#ifndef INSULA_PHYSICS_GHOST_HPP_INCLUDED
#define INSULA_PHYSICS_GHOST_HPP_INCLUDED

#include "object.hpp"
#include "ghost_parameters_fwd.hpp"
#include "broadphase/shared_object_ptr.hpp"
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>

class btBroadphaseProxy;

namespace insula
{
namespace physics
{
/**
	The ghost object tackles the problem of a severe performance
	decrease when many objects are in the world, but almost none are
	colliding with the vehicle, as in the case of many "grass" props
	vs. few collidable props. I suppose the physics world iterates
	through all the bodies and tests for collision, which takes time.

	This class is more "raw", it directly creates a broadphase proxy
	instead of a rigid body and it only has an AABB (a more complex
	shape isn't really needed and is not possible with the dbvt
	broadphase we're using).
 */
class ghost
:
	public physics::object
{
public:
	explicit
	ghost(
		ghost_parameters const &);

	~ghost();
private:
	btCollisionObject collision_object_;
	broadphase::shared_object_ptr broadphase_;
	// We get a pointer from the createProxy function of the broadphase,
	// which hold here.
	btBroadphaseProxy &proxy_;
};
}
}

#endif
