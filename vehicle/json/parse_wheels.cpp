#include "parse_wheels.hpp"
#include "parse_wheel.hpp"
#include "../../stdlib/map.hpp"
#include <sge/parse/json/value.hpp>
#include <sge/parse/json/get.hpp>
#include <sge/parse/json/object.hpp>

insula::physics::vehicle::wheel_info_sequence const
insula::vehicle::json::parse_wheels(
	sge::parse::json::array const &o,
	physics::vehicle::wheel_info const &default_wheel)
{
	return 
		stdlib::map<physics::vehicle::wheel_info_sequence>(
			o.elements,
			[&default_wheel](sge::parse::json::value const &v) 
			{
				return 
					parse_wheel(
						sge::parse::json::get<sge::parse::json::object>(
							v),
						default_wheel);
			});
}
