#ifndef INSULA_PHYSICS_VEHICLE_SHARED_RAYCASTER_PTR_HPP_INCLUDED
#define INSULA_PHYSICS_VEHICLE_SHARED_RAYCASTER_PTR_HPP_INCLUDED

#include "raycaster_fwd.hpp"
#include <memory>

namespace insula
{
namespace physics
{
namespace vehicle
{
typedef
std::shared_ptr<raycaster>
shared_raycaster_ptr;
}
}
}

#endif
