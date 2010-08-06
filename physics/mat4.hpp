#ifndef INSULA_PHYSICS_MAT4_HPP_INCLUDED
#define INSULA_PHYSICS_MAT4_HPP_INCLUDED

#include "scalar.hpp"
#include <fcppt/math/matrix/static.hpp>

namespace insula
{
namespace physics
{
typedef
fcppt::math::matrix::static_<scalar,4,4>::type
mat4;
}
}

#endif
