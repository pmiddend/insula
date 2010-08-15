#include "parse_wheel.hpp"
#include "safe_value.hpp"
#include "parse_vec3.hpp"
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/array.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/text.hpp>

#define INSULA_PHYSICS_JSON_GENERATE_EXTRACTOR(name)\
	r.name(safe_value(o,FCPPT_TEXT(#name),r.name()))

#define INSULA_PHYSICS_JSON_GENERATE_EXTRACTOR_FLOAT(name)\
	r.name(static_cast<scalar>(safe_value(o,FCPPT_TEXT(#name),static_cast<sge::parse::json::float_type>(r.name()))))

insula::physics::vehicle::wheel_info const
insula::physics::json::parse_wheel(
	sge::parse::json::object const &o,
	scalar const scaling,
	vehicle::wheel_info r)
{
	r.position(
		scaling * 
		parse_vec3(
			sge::parse::json::find_member_exn<sge::parse::json::array>(
				o.members,
				FCPPT_TEXT("offset"))));

	INSULA_PHYSICS_JSON_GENERATE_EXTRACTOR(is_front_wheel);
	INSULA_PHYSICS_JSON_GENERATE_EXTRACTOR(gets_steering);
	INSULA_PHYSICS_JSON_GENERATE_EXTRACTOR(gets_engine_force);
	INSULA_PHYSICS_JSON_GENERATE_EXTRACTOR(gets_breaking_force);
	INSULA_PHYSICS_JSON_GENERATE_EXTRACTOR_FLOAT(suspension_stiffness);
	INSULA_PHYSICS_JSON_GENERATE_EXTRACTOR_FLOAT(suspension_compression);
	INSULA_PHYSICS_JSON_GENERATE_EXTRACTOR_FLOAT(suspension_damping);
	INSULA_PHYSICS_JSON_GENERATE_EXTRACTOR_FLOAT(suspension_travel_cm);
	INSULA_PHYSICS_JSON_GENERATE_EXTRACTOR_FLOAT(suspension_rest_length);
	INSULA_PHYSICS_JSON_GENERATE_EXTRACTOR_FLOAT(damping_relaxation);
	INSULA_PHYSICS_JSON_GENERATE_EXTRACTOR_FLOAT(damping_compression);
	INSULA_PHYSICS_JSON_GENERATE_EXTRACTOR_FLOAT(friction_slip);
	INSULA_PHYSICS_JSON_GENERATE_EXTRACTOR_FLOAT(roll_influence);
	INSULA_PHYSICS_JSON_GENERATE_EXTRACTOR_FLOAT(maximum_suspension_force);
	
	return r;
}
