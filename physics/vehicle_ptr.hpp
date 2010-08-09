#ifndef INSULA_PHYSICS_VEHICLE_PTR_HPP_INCLUDED
#define INSULA_PHYSICS_VEHICLE_PTR_HPP_INCLUDED

#include "vehicle_fwd.hpp"
#include <memory>

namespace insula
{
namespace physics
{
typedef
std::shared_ptr<vehicle>
vehicle_ptr;
}
}

#endif
