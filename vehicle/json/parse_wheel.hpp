#ifndef INSULA_VEHICLE_JSON_PARSE_WHEEL_HPP_INCLUDED
#define INSULA_VEHICLE_JSON_PARSE_WHEEL_HPP_INCLUDED

#include "../../physics/vehicle/wheel_info.hpp"
#include <sge/parse/json/object.hpp>

namespace insula
{
namespace vehicle
{
namespace json
{
physics::vehicle::wheel_info const
parse_wheel(
	sge::parse::json::object const &o,
	physics::vehicle::wheel_info);
}
}
}

#endif
