#ifndef INSULA_PHYSICS_SHAPE_FROM_MODEL_HPP_INCLUDED
#define INSULA_PHYSICS_SHAPE_FROM_MODEL_HPP_INCLUDED

#include "shape_ptr.hpp"
#include "model_approximation.hpp"
#include "../model/object_fwd.hpp"

namespace insula
{
namespace physics
{
shape_ptr const
shape_from_model(
	model::object &,
	model_approximation const &);
}
}

#endif
