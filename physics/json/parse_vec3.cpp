#include "parse_vec3.hpp"
#include "../scalar.hpp"
#include <fcppt/math/vector/basic_impl.hpp>
#include <sge/parse/json/float_type.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/get.hpp>

insula::physics::vec3 const
insula::physics::json::parse_vec3(
	sge::parse::json::array const &a)
{
	return 
		vec3(
			static_cast<scalar>(
				sge::parse::json::get<sge::parse::json::float_type>(
					a.elements[0])),
			static_cast<scalar>(
				sge::parse::json::get<sge::parse::json::float_type>(
					a.elements[1])),
			static_cast<scalar>(
				sge::parse::json::get<sge::parse::json::float_type>(
					a.elements[2])));
}
