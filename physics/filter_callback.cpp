#include "filter_callback.hpp"
#include "object.hpp"
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>

namespace
{
bool 
is_ghost(
	btBroadphaseProxy *p)
{
	return 
		static_cast<insula::physics::object *>(
			static_cast<btCollisionObject *>(
				p->m_clientObject)->getUserPointer())->type() == 
			insula::physics::object_type::ghost;
}
}

insula::physics::filter_callback::filter_callback(
	btOverlappingPairCache &_cache)
:
	cache_(
		_cache)
{
	cache_.setOverlapFilterCallback(
		this);
}

bool
insula::physics::filter_callback::needBroadphaseCollision(
	btBroadphaseProxy * const proxy0,
	btBroadphaseProxy * const proxy1) const
{
	if(is_ghost(proxy0) || is_ghost(proxy1))
		return false;

	// This is the default behaviour
	bool collides = 
		(proxy0->m_collisionFilterGroup & proxy1->m_collisionFilterMask) != 0;

	collides = 
		collides && 
		(proxy1->m_collisionFilterGroup & proxy0->m_collisionFilterMask);

	return collides;
}

insula::physics::filter_callback::~filter_callback()
{
	cache_.setOverlapFilterCallback(
		0);
}
