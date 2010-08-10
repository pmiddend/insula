#ifndef INSULA_GRAPHICS_CAMERA_LOCK_TO_HPP_INCLUDED
#define INSULA_GRAPHICS_CAMERA_LOCK_TO_HPP_INCLUDED

#include "object_fwd.hpp"
#include "../gizmo.hpp"
#include "../scalar.hpp"

namespace insula
{
namespace graphics
{
namespace camera
{
void
lock_to(
	object &,
	gizmo const &,
	scalar distance,
	scalar angle);
}
}
}

#endif
