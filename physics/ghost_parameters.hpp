#ifndef INSULA_PHYSICS_GHOST_PARAMETERS_HPP_INCLUDED
#define INSULA_PHYSICS_GHOST_PARAMETERS_HPP_INCLUDED

#include "box.hpp"
#include "broadphase/shared_object_ptr.hpp"
#include <fcppt/math/box/basic_impl.hpp>

class btDbvtBroadphase;

namespace insula
{
namespace physics
{
class ghost_parameters
{
public:
	broadphase::shared_object_ptr broadphase;
	box aabb;

	explicit
	ghost_parameters(
		broadphase::shared_object_ptr broadphase,
		box const &aabb)
	:
		broadphase(broadphase),
		aabb(aabb)
	{
	}
};
}
}

#endif
