#ifndef INSULA_SKYDOME_SPHERE_POINT_HPP_INCLUDED
#define INSULA_SKYDOME_SPHERE_POINT_HPP_INCLUDED

#include "../graphics/scalar.hpp"
#include "vf/packed_position.hpp"

namespace insula
{
namespace skydome
{
vf::packed_position
sphere_point(
	graphics::scalar radius,
	graphics::scalar lat,
	graphics::scalar lon);
}
}

#endif
