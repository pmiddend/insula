#ifndef INSULA_PHYSICS_MAT4_SEQUENCE_HPP_INCLUDED
#define INSULA_PHYSICS_MAT4_SEQUENCE_HPP_INCLUDED

#include "mat4.hpp"
#include <vector>

namespace insula
{
namespace physics
{
// This is used by the vehicle to report the wheels' transforms
typedef
std::vector
<
	mat4
>
mat4_sequence;
}
}

#endif
