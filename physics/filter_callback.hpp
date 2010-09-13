#ifndef INSULA_PHYSICS_FILTER_CALLBACK_HPP_INCLUDED
#define INSULA_PHYSICS_FILTER_CALLBACK_HPP_INCLUDED

#include <BulletCollision/BroadphaseCollision/btOverlappingPairCache.h>

namespace insula
{
namespace physics
{
class filter_callback
:
	public btOverlapFilterCallback
{
public:
	explicit
	filter_callback(
		btOverlappingPairCache &);
	
	bool
	needBroadphaseCollision(
		btBroadphaseProxy *,
		btBroadphaseProxy *) const;

	~filter_callback();
private:
	btOverlappingPairCache &cache_;
};
}
}

#endif
