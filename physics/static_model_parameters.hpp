#ifndef INSULA_PHYSICS_STATIC_MODEL_PARAMETERS_HPP_INCLUDED
#define INSULA_PHYSICS_STATIC_MODEL_PARAMETERS_HPP_INCLUDED

#include "world_fwd.hpp"
#include "object_type.hpp"
#include "vec3.hpp"
#include "mat3.hpp"
#include "shared_shape_ptr.hpp"
#include "solidity.hpp"
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>

namespace insula
{
namespace physics
{
class static_model_parameters
{
public:
	world &world_;
	object_type::type type;
	vec3 position;
	mat3 transformation;
	shared_shape_ptr shape;
	solidity::type solidity;

	explicit
	static_model_parameters(
		world &world_,
		object_type::type type,
		vec3 position,
		mat3 transformation,
		shared_shape_ptr shape,
		solidity::type solidity)
	:
		world_(world_),
		type(type),
		position(position),
		transformation(transformation),
		shape(shape),
		solidity(solidity)
	{
	}
};
}
}

#endif
