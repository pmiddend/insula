#ifndef INSULA_PHYSICS_BULLET_TO_MAT4_HPP_INCLUDED
#define INSULA_PHYSICS_BULLET_TO_MAT4_HPP_INCLUDED

#include "mat4.hpp"
#include <fcppt/math/matrix/basic_impl.hpp>

class btMatrix3x3;

namespace insula
{
namespace physics
{
mat4 const
bullet_to_mat4(
	btMatrix3x3 const &);
}
}

#endif
