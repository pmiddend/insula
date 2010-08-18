#include "parse_wheels.hpp"
#include "parse_wheel.hpp"
#include "../../stdlib/map.hpp"
#include <sge/parse/json/value.hpp>
#include <sge/parse/json/get.hpp>
#include <sge/parse/json/object.hpp>

insula::physics::vehicle::wheel_info_sequence const
insula::physics::json::parse_wheels(
	sge::parse::json::array const &o,
	scalar const scaling,
	vehicle::wheel_info const &default_wheel)
{
	return 
		stdlib::map<vehicle::wheel_info_sequence>(
			o.elements,
			[&scaling,&default_wheel](sge::parse::json::value const &v) 
			{
				return 
					parse_wheel(
						sge::parse::json::get<sge::parse::json::object>(
							v),
						scaling,
						default_wheel);
			});
}
