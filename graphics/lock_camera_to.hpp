#ifndef INSULA_GRAPHICS_LOCK_CAMERA_TO_HPP_INCLUDED
#define INSULA_GRAPHICS_LOCK_CAMERA_TO_HPP_INCLUDED

#include "camera_fwd.hpp"
#include "gizmo.hpp"
#include "scalar.hpp"

namespace insula
{
namespace graphics
{
void
lock_camera_to(
	camera &,
	vec3 const &position,
	gizmo const &,
	scalar distance,
	scalar angle);
}
}

#endif
