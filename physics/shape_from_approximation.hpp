#ifndef INSULA_PHYSICS_SHAPE_FROM_APPROXIMATION_HPP_INCLUDED
#define INSULA_PHYSICS_SHAPE_FROM_APPROXIMATION_HPP_INCLUDED

#include "shared_shape_ptr.hpp"
#include "approximation/variant.hpp"

namespace insula
{
namespace physics
{
shared_shape_ptr const
shape_from_approximation(
	approximation::variant const &);
}
}

#endif
