#include "parse_vehicle.hpp"
#include "parse_wheel.hpp"
#include "parse_wheels.hpp"
#include "../../create_path.hpp"
#include "../../media_path.hpp"
#include "../../physics/vehicle/wheel_info.hpp"
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/parse_file.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/float_type.hpp>
#include <sge/parse/json/string.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/systems/instance.hpp>
#include <sge/model/loader.hpp>
#include <sge/audio/player.hpp>
#include <sge/audio/multi_loader.hpp>
#include <sge/exception.hpp>
#include <fcppt/text.hpp>

insula::vehicle::parameters const
insula::vehicle::json::parse_vehicle(
	scene::manager &scene_manager,
	sge::parse::json::object const &json_file,
	// Passing this is _much_ easier than passing all the systems
	// separate
	sge::systems::instance const &sys,
	graphics::camera::object &camera,
	graphics::shader::object &model_shader,
	physics::world &physics_world,
	physics::vec3 const &position,
	input_delegator &input_delegator_,
	console::object &console,
	graphics::scalar const camera_distance,
	graphics::scalar const camera_angle,
	height_map::object &height_map)
{
	physics::vehicle::wheel_info const default_wheel = 
		parse_wheel(
			sge::parse::json::find_member_exn<sge::parse::json::object>(
				json_file.members,
				FCPPT_TEXT("default_wheel")),
			physics::vehicle::wheel_info());

	sge::parse::json::object const 
		chassis_model = 
			sge::parse::json::find_member_exn<sge::parse::json::object>(
				json_file.members,
				FCPPT_TEXT("chassis")),
		wheel_model = 
			sge::parse::json::find_member_exn<sge::parse::json::object>(
				json_file.members,
				FCPPT_TEXT("wheel"));

	return 
		parameters(
			scene_manager,
			sys.md3_loader()->load(
				create_path(
					sge::parse::json::find_member_exn<sge::parse::json::string>(
						chassis_model.members,
						FCPPT_TEXT("model")),
					FCPPT_TEXT("models/vehicles")),
				sge::model::load_flags::switch_yz),
			sys.md3_loader()->load(
				create_path(
					sge::parse::json::find_member_exn<sge::parse::json::string>(
						wheel_model.members,
						FCPPT_TEXT("model")),
					FCPPT_TEXT("models/vehicles")),
				sge::model::load_flags::switch_yz),
			sys.renderer(),
			sys.image_loader(),
			camera,
			model_shader,
			create_path(
				sge::parse::json::find_member_exn<sge::parse::json::string>(
					chassis_model.members,
					FCPPT_TEXT("texture")),
				FCPPT_TEXT("textures/vehicles")),
			create_path(
				sge::parse::json::find_member_exn<sge::parse::json::string>(
					wheel_model.members,
					FCPPT_TEXT("texture")),
				FCPPT_TEXT("textures/vehicles")),
			physics_world,
			static_cast<physics::scalar>(
				sge::parse::json::find_member_exn<sge::parse::json::float_type>(
					json_file.members,
					FCPPT_TEXT("mass"))),
			static_cast<physics::scalar>(
				sge::parse::json::find_member_exn<sge::parse::json::float_type>(
					json_file.members,
					FCPPT_TEXT("chassis_center_of_mass_shift"))),
			static_cast<physics::scalar>(
				sge::parse::json::find_member_exn<sge::parse::json::float_type>(
					json_file.members,
					FCPPT_TEXT("steering_clamp"))),
			static_cast<physics::scalar>(
				sge::parse::json::find_member_exn<sge::parse::json::float_type>(
					json_file.members,
					FCPPT_TEXT("steering_speed"))),
			static_cast<physics::scalar>(
				sge::parse::json::find_member_exn<sge::parse::json::float_type>(
					json_file.members,
					FCPPT_TEXT("braking_speed"))),
			static_cast<physics::scalar>(
				sge::parse::json::find_member_exn<sge::parse::json::float_type>(
					json_file.members,
					FCPPT_TEXT("engine_speed"))),
			static_cast<physics::scalar>(
				sge::parse::json::find_member_exn<sge::parse::json::float_type>(
					json_file.members,
					FCPPT_TEXT("max_engine_force"))),
			static_cast<physics::scalar>(
				sge::parse::json::find_member_exn<sge::parse::json::float_type>(
					json_file.members,
					FCPPT_TEXT("max_breaking_force"))),
			static_cast<physics::scalar>(
				sge::parse::json::find_member_exn<sge::parse::json::float_type>(
					json_file.members,
					FCPPT_TEXT("max_speed"))),
			static_cast<physics::scalar>(
				sge::parse::json::find_member_exn<sge::parse::json::float_type>(
					json_file.members,
					FCPPT_TEXT("track_connection"))),
			position,
			parse_wheels(
				sge::parse::json::find_member_exn<sge::parse::json::array>(
					json_file.members,
					FCPPT_TEXT("wheels")),
				default_wheel),
			input_delegator_,
			console,
			camera_distance,
			camera_angle,
			sys.audio_player(),
			sys.audio_player()->create_buffer(
				sys.audio_loader().load(
					create_path(
						sge::parse::json::find_member_exn<sge::parse::json::string>(
							json_file.members,
							FCPPT_TEXT("engine_sound")),
						FCPPT_TEXT("sounds")))),
			sys.audio_player()->create_buffer(
				sys.audio_loader().load(
					media_path()/FCPPT_TEXT("sounds")/FCPPT_TEXT("skid.wav"))),
			height_map);
}
