#ifndef INSULA_PHYSICS_JSON_PARSE_VEC3_HPP_INCLUDED
#define INSULA_PHYSICS_JSON_PARSE_VEC3_HPP_INCLUDED

#include "../vec3.hpp"
#include <sge/parse/json/array.hpp>

namespace insula
{
namespace physics
{
namespace json
{
vec3 const
parse_vec3(
	sge::parse::json::array const &);
}
}
}

#endif
