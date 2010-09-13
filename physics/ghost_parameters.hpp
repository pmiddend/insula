#ifndef INSULA_PHYSICS_GHOST_PARAMETERS_HPP_INCLUDED
#define INSULA_PHYSICS_GHOST_PARAMETERS_HPP_INCLUDED

#include "object_type.hpp"
#include "vec3.hpp"
#include "mat3.hpp"
#include "scalar.hpp"
#include "world_fwd.hpp"
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>

namespace insula
{
namespace physics
{
class ghost_parameters
{
public:
	world &world_;
	vec3 position;
	mat3 transformation;
	scalar radius;

	explicit
	ghost_parameters(
		world &world_,
		vec3 position,
		mat3 transformation,
		scalar radius)
	:
		world_(world_),
		position(position),
		transformation(transformation),
		radius(radius)
	{
	}
};
}
}

#endif
