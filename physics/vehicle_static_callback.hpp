#ifndef INSULA_PHYSICS_VEHICLE_STATIC_CALLBACK_HPP_INCLUDED
#define INSULA_PHYSICS_VEHICLE_STATIC_CALLBACK_HPP_INCLUDED

#include "vehicle_static_callback_fn.hpp"
#include <functional>

namespace insula
{
namespace physics
{
typedef
std::function<vehicle_static_callback_fn>
vehicle_static_callback;
}
}

#endif
