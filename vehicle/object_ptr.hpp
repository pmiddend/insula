#ifndef INSULA_VEHICLE_OBJECT_PTR_HPP_INCLUDED
#define INSULA_VEHICLE_OBJECT_PTR_HPP_INCLUDED

#include "object_fwd.hpp"
#include <memory>

namespace insula
{
namespace vehicle
{
typedef
std::shared_ptr<object>
object_ptr;
}
}

#endif
