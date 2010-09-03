#ifndef INSULA_PHYSICS_CULLER_HPP_INCLUDED
#define INSULA_PHYSICS_CULLER_HPP_INCLUDED

#include "iteration.hpp"
#include <BulletCollision/BroadphaseCollision/btDbvt.h>

namespace insula
{
namespace physics
{
class culler
:
	public btDbvt::ICollide
{
public:
	explicit
	culler(
		iteration);
	
	void
	Process(
		btDbvtNode const *,
		btDbvtNode const *);

	void 
	Process(
		btDbvtNode const *);

	void 
	Process(
		btDbvtNode const *,
		btScalar);

	bool 
	Descent(
		btDbvtNode const *);

	bool
	AllLeaves(
		btDbvtNode const *);
private:
	iteration iteration_;
};
}
}

#endif
