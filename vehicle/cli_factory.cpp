#include "cli_factory.hpp"
#include "object.hpp"
#include "input.hpp"
#include "json/parse_vehicle.hpp"
#include "../create_path.hpp"
#include "../get_option.hpp"
#include "../graphics/scalar.hpp"
#include <sge/parse/json/parse_file_exn.hpp>
#include <sge/parse/json/array.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/text.hpp>

insula::vehicle::parameters const
insula::vehicle::cli_factory(
	boost::program_options::variables_map const &vm,
	scene::manager &scene_manager,
	sge::systems::instance const &sys,
	graphics::camera::object &camera,
	graphics::shader::object &model_shader,
	physics::world &physics_world,
	physics::vec3 const &position,
	input_delegator &input_delegator_,
	console::object &console)
{
	return 
		json::parse_vehicle(
			scene_manager,
			sge::parse::json::parse_file_exn(
				create_path(
					get_option<fcppt::string>(vm,"vehicle-file"),
					FCPPT_TEXT("vehicles"))),
			sys,
			camera,
			model_shader,
			physics_world,
			position,
			input_delegator_,
			console,
			get_option<graphics::scalar>(vm,"vehicle-camera-distance"),
			fcppt::math::deg_to_rad( 
				get_option<graphics::scalar>(vm,"vehicle-camera-angle")));
}
