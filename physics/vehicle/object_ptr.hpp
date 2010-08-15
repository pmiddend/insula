#ifndef INSULA_PHYSICS_VEHICLE_OBJECT_PTR_HPP_INCLUDED
#define INSULA_PHYSICS_VEHICLE_OBJECT_PTR_HPP_INCLUDED

#include "object_fwd.hpp"
#include <memory>

namespace insula
{
namespace physics
{
namespace vehicle
{
typedef
std::shared_ptr<object>
object_ptr;
}
}
}

#endif
