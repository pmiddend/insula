#ifndef INSULA_WATER_MIRROR_CAMERA_HPP_INCLUDED
#define INSULA_WATER_MIRROR_CAMERA_HPP_INCLUDED

#include "../graphics/vec3.hpp"
#include "../graphics/gizmo.hpp"
#include "../graphics/scalar.hpp"
#include <fcppt/math/vector/basic_impl.hpp>
#include <tuple>

namespace insula
{
namespace water
{
std::tuple<graphics::vec3,graphics::gizmo> const
mirror_camera(
	graphics::vec3 const &old_position,
	graphics::gizmo const &axes,
	graphics::scalar const water_height);
}
}

#endif
