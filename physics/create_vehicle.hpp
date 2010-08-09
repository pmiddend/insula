#ifndef INSULA_PHYSICS_CREATE_VEHICLE_HPP_INCLUDED
#define INSULA_PHYSICS_CREATE_VEHICLE_HPP_INCLUDED

#include "vehicle.hpp"
#include <memory>

namespace insula
{
namespace physics
{
std::shared_ptr<vehicle> const
create_vehicle();
}
}

#endif
