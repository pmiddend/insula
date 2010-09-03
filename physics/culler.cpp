#include "culler.hpp"
#include "object.hpp"
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletCollision/BroadphaseCollision/btBroadphaseProxy.h>
#include <fcppt/assert.hpp>
#include <fcppt/io/cout.hpp>

insula::physics::culler::culler(
	iteration const _iteration)
:
	iteration_(
		_iteration)
{
}

// The default implementation also does nothing here
void
insula::physics::culler::Process(
	btDbvtNode const *,
	btDbvtNode const *)
{
}

// The default implementation does nothing here
void 
insula::physics::culler::Process(
	btDbvtNode const *node)
{
	FCPPT_ASSERT(node);
	FCPPT_ASSERT(node->data);
	FCPPT_ASSERT(!node->isinternal());

	//fcppt::io::cout << "Encountered node with handle: " << node->data << "\n";

	// The DbvtBroadphase puts the broadphase proxies to the data field
	// in the tree (apparently). 
	// btCollisionWorld replaces the m_clientObject pointer by the
	// btCollisionObject
	// Finally, _we_ set the collision object's user pointer.
	static_cast<object *>(
		static_cast<btCollisionObject *>(
			static_cast<btBroadphaseProxy *>(
				node->data)->m_clientObject)->getUserPointer())->last_seen(iteration_);
}

// The default implementation calls Process(n) here
void 
insula::physics::culler::Process(
	btDbvtNode const *n,
	btScalar /*depth*/)
{ 
	Process(
		n); 
}

// The default implementation just returns true here
bool 
insula::physics::culler::Descent(
	btDbvtNode const *)
{ 
	FCPPT_ASSERT(false);
	return true;
}

// The default implementation just returns true here
bool 
insula::physics::culler::AllLeaves(
	btDbvtNode const *)
{ 
	return true;
}
