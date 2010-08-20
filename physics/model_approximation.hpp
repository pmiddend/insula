#ifndef INSULA_PHYSICS_MODEL_APPROXIMATION_HPP_INCLUDED
#define INSULA_PHYSICS_MODEL_APPROXIMATION_HPP_INCLUDED

#include "scalar.hpp"

namespace insula
{
namespace physics
{
class model_approximation
{
public:
	enum type
	{
		box,
		sphere,
		cylinder_x,
		cylinder_y,
		cylinder_z,
		exact
	};

	type t;
	scalar scaling;

	explicit
	model_approximation(
		type,
		scalar);
};
}
}

#endif
