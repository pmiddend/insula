#ifndef INSULA_PHYSICS_JSON_PARSE_WHEEL_HPP_INCLUDED
#define INSULA_PHYSICS_JSON_PARSE_WHEEL_HPP_INCLUDED

#include "../wheel_info.hpp"
#include "../scalar.hpp"
#include <sge/parse/json/object.hpp>

namespace insula
{
namespace physics
{
namespace json
{
wheel_info const
parse_wheel(
	sge::parse::json::object const &o,
	scalar const scaling,
	wheel_info);
}
}
}

#endif
