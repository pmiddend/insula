#ifndef INSULA_PHYSICS_JSON_PARSE_WHEELS_HPP_INCLUDED
#define INSULA_PHYSICS_JSON_PARSE_WHEELS_HPP_INCLUDED

#include "../vehicle/wheel_info_sequence.hpp"
#include "../vehicle/wheel_info.hpp"
#include <sge/parse/json/array.hpp>

namespace insula
{
namespace physics
{
namespace json
{
vehicle::wheel_info_sequence const
parse_wheels(
	sge::parse::json::array const &o,
	vehicle::wheel_info const &default_wheel);
}
}
}

#endif
