#ifndef INSULA_PHYSICS_SHAPE_FROM_MODEL_HPP_INCLUDED
#define INSULA_PHYSICS_SHAPE_FROM_MODEL_HPP_INCLUDED

#include "shared_shape_ptr.hpp"
#include "approximation/numeric_value.hpp"
#include "../model/object_fwd.hpp"

namespace insula
{
namespace physics
{
shared_shape_ptr const
shape_from_model(
	model::object &,
	approximation::numeric_value::type);
}
}

#endif
