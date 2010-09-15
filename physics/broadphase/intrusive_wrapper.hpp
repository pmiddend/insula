#ifndef INSULA_PHYSICS_BROADPHASE_INTRUSIVE_WRAPPER_HPP_INCLUDED
#define INSULA_PHYSICS_BROADPHASE_INTRUSIVE_WRAPPER_HPP_INCLUDED

#include <boost/intrusive/list_hook.hpp>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>

namespace insula
{
namespace physics
{
namespace broadphase
{
/**
	See the manager for how this is used
	*/
class intrusive_wrapper
:
	public btDbvtBroadphase,
	public 
		boost::intrusive::list_base_hook
		<
			boost::intrusive::link_mode<boost::intrusive::auto_unlink> 
		>
{
};
}
}
}

#endif
