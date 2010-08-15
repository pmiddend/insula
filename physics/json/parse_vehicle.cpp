#include "parse_vehicle.hpp"
#include "parse_wheel.hpp"
#include "parse_model.hpp"
#include "parse_wheels.hpp"
#include "../vehicle.hpp"
#include "../scalar.hpp"
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/parse_file.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/float_type.hpp>
#include <sge/exception.hpp>
#include <fcppt/text.hpp>

insula::physics::vehicle_ptr const
insula::physics::json::parse_vehicle(
	fcppt::filesystem::path const &fn,
	world &w,
	vec3 const &position,
	sge::renderer::device_ptr const rend,
	sge::image::multi_loader &il,
	sge::model::loader_ptr const model_loader,
	graphics::shader &shader,
	graphics::camera::object &cam)
{
	sge::parse::json::object json_file;
	if (!sge::parse::json::parse_file(fn,json_file))
		throw sge::exception(FCPPT_TEXT("Error parsing file: ")+fn.string());

	scalar const scaling = 
		static_cast<scalar>(
			sge::parse::json::find_member_exn<sge::parse::json::float_type>(
				json_file.members,
				FCPPT_TEXT("scaling")));

	wheel_info const default_wheel = 
		parse_wheel(
			sge::parse::json::find_member_exn<sge::parse::json::object>(
				json_file.members,
				FCPPT_TEXT("default_wheel")),
			scaling,
			wheel_info());

	return 
		std::make_shared<vehicle>(
			w,
			rend,
			parse_model(
				sge::parse::json::find_member_exn<sge::parse::json::object>(
					json_file.members,
					FCPPT_TEXT("chassis")),
				model_loader,
				il,
				rend,
				cam,
				shader,
				scaling),
			static_cast<scalar>(
				sge::parse::json::find_member_exn<sge::parse::json::float_type>(
					json_file.members,
					FCPPT_TEXT("mass"))),
			static_cast<scalar>(
				sge::parse::json::find_member_exn<sge::parse::json::float_type>(
					json_file.members,
					FCPPT_TEXT("chassis_center_of_mass_shift"))),
			static_cast<scalar>(
				sge::parse::json::find_member_exn<sge::parse::json::float_type>(
					json_file.members,
					FCPPT_TEXT("steering_clamp"))),
			position,
			static_cast<scalar>(
				sge::parse::json::find_member_exn<sge::parse::json::float_type>(
					json_file.members,
					FCPPT_TEXT("max_engine_force"))),
			static_cast<scalar>(
				sge::parse::json::find_member_exn<sge::parse::json::float_type>(
					json_file.members,
					FCPPT_TEXT("max_breaking_force"))),
			static_cast<scalar>(
				sge::parse::json::find_member_exn<sge::parse::json::float_type>(
					json_file.members,
					FCPPT_TEXT("max_speed"))),
			parse_model(
				sge::parse::json::find_member_exn<sge::parse::json::object>(
					json_file.members,
					FCPPT_TEXT("wheel")),
				model_loader,
				il,
				rend,
				cam,
				shader,
				scaling),
			parse_wheels(
				sge::parse::json::find_member_exn<sge::parse::json::array>(
					json_file.members,
					FCPPT_TEXT("wheels")),
				scaling,
				default_wheel));
}
