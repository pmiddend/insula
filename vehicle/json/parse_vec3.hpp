#ifndef INSULA_VEHICLE_JSON_PARSE_VEC3_HPP_INCLUDED
#define INSULA_VEHICLE_JSON_PARSE_VEC3_HPP_INCLUDED

#include "../../physics/vec3.hpp"
#include <sge/parse/json/array.hpp>

namespace insula
{
namespace vehicle
{
namespace json
{
physics::vec3 const
parse_vec3(
	sge::parse::json::array const &);
}
}
}

#endif
