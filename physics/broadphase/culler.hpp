#ifndef INSULA_PHYSICS_BROADPHASE_CULLER_HPP_INCLUDED
#define INSULA_PHYSICS_BROADPHASE_CULLER_HPP_INCLUDED

#include "iteration.hpp"
#include <BulletCollision/BroadphaseCollision/btDbvt.h>

namespace insula
{
namespace physics
{
namespace broadphase
{
/**
	Cullig works as follows: physics::world has the "update_visibility"
	function which takes the camera and extracts the frustum planes from
	it. Then, it calls the frustum culling function of the dbvt
	broadphase. This function gets an ICollide class (the class you see
	below) to trigger actions while it traverses the dbvt tree.

	This class updates the "last seen" number of the objects it
	encounters and sets it to the current iteration. You can then
	compare the current iteration with the last seen number and see if
	an object is _currently_ visible. 
	
	An alternative to the "last seen" approach would be to give a
	physics object a "bool visible;". But then, before testing an
	object's visibility, we have to set all of those booleans to
	"false", since we're only informed about an object being visible,
	not when it becomes invisible again.
 */
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
}

#endif
