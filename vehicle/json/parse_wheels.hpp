#ifndef INSULA_VEHICLE_JSON_PARSE_WHEELS_HPP_INCLUDED
#define INSULA_VEHICLE_JSON_PARSE_WHEELS_HPP_INCLUDED

#include "../../physics/vehicle/wheel_info_sequence.hpp"
#include "../../physics/vehicle/wheel_info.hpp"
#include <sge/parse/json/array.hpp>

namespace insula
{
namespace vehicle
{
namespace json
{
physics::vehicle::wheel_info_sequence const
parse_wheels(
	sge::parse::json::array const &o,
	physics::vehicle::wheel_info const &default_wheel);
}
}
}

#endif
