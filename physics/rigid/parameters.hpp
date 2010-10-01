#ifndef INSULA_PHYSICS_RIGID_PARAMETERS_HPP_INCLUDED
#define INSULA_PHYSICS_RIGID_PARAMETERS_HPP_INCLUDED

#include "solidity.hpp"
#include "../world_fwd.hpp"
#include "../object_type.hpp"
#include "../vec3.hpp"
#include "../mat3.hpp"
#include "../shared_shape_ptr.hpp"
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/optional.hpp>

namespace insula
{
namespace physics
{
namespace rigid
{
class parameters
{
public:
	world &world_;
	object_type::type type;
	vec3 position;
	mat3 transformation;
	shared_shape_ptr shape;
	solidity::type solidity;
	fcppt::optional<scalar> mass;
	vec3 linear_velocity;

	explicit
	parameters(
		world &world_,
		object_type::type type,
		vec3 position,
		mat3 transformation,
		shared_shape_ptr shape,
		solidity::type solidity,
		fcppt::optional<scalar> mass,
		vec3 const &linear_velocity)
	:
		world_(world_),
		type(type),
		position(position),
		transformation(transformation),
		shape(shape),
		solidity(solidity),
		mass(mass),
		linear_velocity(linear_velocity)
	{
	}
};
}
}
}

#endif
