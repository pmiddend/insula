#include "ghost.hpp"
#include "ghost_parameters.hpp"
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>

namespace
{
// Helper functions needed to give the box to createProxy
btVector3
box_min(
	insula::physics::box const &b)
{
	return 
		btVector3(
			b.left(),
			b.bottom(),
			b.back());
}

// Helper functions needed to give the box to createProxy
btVector3
box_max(
	insula::physics::box const &b)
{
	return 
		btVector3(
			b.right(),
			b.top(),
			b.front());
}
}

insula::physics::ghost::ghost(
	ghost_parameters const &params)
:
	object(
		object_type::ghost),
	collision_object_(),
	broadphase_(
		params.broadphase),
	proxy_(
		*(broadphase_->createProxy(
			box_min(
				params.aabb),
			box_max(
				params.aabb),
			// shapeType, unused by broadphase (the dispatcher uses it)
			0,
			// user pointer
			&collision_object_,
			// group and mask (the CDDemo uses 1,1, too , so I copied it)
			1,
			1,
			// dispatcher, unused
			0,
			// sapProxy, unused
			0)))
{
}

insula::physics::ghost::~ghost()
{
	broadphase_->destroyProxy(
		&proxy_,
		// Dispatcher
		0);
}
