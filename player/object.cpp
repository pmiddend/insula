#include "object.hpp"
#include "parameters.hpp"
#include "../height_map/random_point.hpp"
#include "../json/parse_vector.hpp"
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/float_type.hpp>
#include <fcppt/text.hpp>

insula::player::object::object(
	parameters const &params)
:
	character_controller_(
		static_cast<physics::scalar>(
			sge::parse::json::find_member_exn<sge::parse::json::float_type>(
				params.config_file.members,
				FCPPT_TEXT("width"))),
		static_cast<physics::scalar>(
			sge::parse::json::find_member_exn<sge::parse::json::float_type>(
				params.config_file.members,
				FCPPT_TEXT("height"))),
		height_map::random_point(
			params.height_map,
			params.water_level,
			params.rng_engine,
			json::parse_vector<height_map::scalar,2,sge::parse::json::float_type>(
				sge::parse::json::find_member_exn<sge::parse::json::array>(
					params.config_file.members,
					FCPPT_TEXT("flatness-range")))))
{
}

insula::physics::gizmo const
insula::player::object::gizmo() const
{
	return 
		character_controller_.gizmo();
}
