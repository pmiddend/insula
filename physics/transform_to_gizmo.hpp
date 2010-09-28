#ifndef INSULA_PHYSICS_TRANSFORM_TO_GIZMO_HPP_INCLUDED
#define INSULA_PHYSICS_TRANSFORM_TO_GIZMO_HPP_INCLUDED

#include "gizmo.hpp"
#include <LinearMath/btTransform.h>

namespace insula
{
namespace physics
{
gizmo const
transform_to_gizmo(
	btTransform const &);
}
}

#endif
