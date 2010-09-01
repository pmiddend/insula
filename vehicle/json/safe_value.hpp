#ifndef INSULA_VEHICLE_JSON_SAFE_VALUE_HPP_INCLUDED
#define INSULA_VEHICLE_JSON_SAFE_VALUE_HPP_INCLUDED

#include <sge/parse/json/object.hpp>
#include <sge/parse/json/find_member.hpp>
#include <fcppt/string.hpp>

namespace insula
{
namespace vehicle
{
namespace json
{
template<typename T>
T const
safe_value(
	sge::parse::json::object const &o,
	fcppt::string const &name,
	T const &default_value)
{
	T const *f = 
		sge::parse::json::find_member<T>(
			o.members,
			name);

	return f ? *f : default_value;
}

}
}
}

#endif
