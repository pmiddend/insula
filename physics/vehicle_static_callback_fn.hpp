#ifndef INSULA_PHYSICS_VEHICLE_STATIC_CALLBACK_FN_HPP_INCLUDED
#define INSULA_PHYSICS_VEHICLE_STATIC_CALLBACK_FN_HPP_INCLUDED

#include "vehicle/object_fwd.hpp"
#include "static_model_fwd.hpp"

namespace insula
{
namespace physics
{
typedef
void
vehicle_static_callback_fn(
	vehicle::object &,
	static_model &);
}
}

#endif
