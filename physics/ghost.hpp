#ifndef INSULA_PHYSICS_GHOST_HPP_INCLUDED
#define INSULA_PHYSICS_GHOST_HPP_INCLUDED

#include "object.hpp"
#include "ghost_parameters_fwd.hpp"
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>

class btCollisionWorld;

namespace insula
{
namespace physics
{
class ghost
:
	public physics::object,
	public btCollisionObject
{
public:
	explicit
	ghost(
		ghost_parameters const &);

	~ghost();
private:
	btCollisionWorld &world_;
};
}
}

#endif
