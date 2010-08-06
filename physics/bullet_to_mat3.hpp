#ifndef INSULA_PHYSICS_BULLET_TO_MAT3_HPP_INCLUDED
#define INSULA_PHYSICS_BULLET_TO_MAT3_HPP_INCLUDED

#include "mat3.hpp"
#include <fcppt/math/matrix/basic_impl.hpp>

class btMatrix3x3;

namespace insula
{
namespace physics
{
mat3 const
bullet_to_mat3(
	btMatrix3x3 const &);
}
}

#endif
