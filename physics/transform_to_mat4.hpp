#ifndef INSULA_PHYSICS_TRANSFORM_TO_MAT4_HPP_INCLUDED
#define INSULA_PHYSICS_TRANSFORM_TO_MAT4_HPP_INCLUDED

#include "../graphics/mat4.hpp"
#include <fcppt/math/matrix/basic_impl.hpp>

class btTransform;

namespace insula
{
namespace physics
{
graphics::mat4 const
transform_to_mat4(
	btTransform const &);
}
}

#endif
