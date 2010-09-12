#ifndef INSULA_VEHICLE_CLI_FACTORY_HPP_INCLUDED
#define INSULA_VEHICLE_CLI_FACTORY_HPP_INCLUDED

#include "parameters.hpp"
#include "../graphics/camera/object_fwd.hpp"
#include "../scene/manager_fwd.hpp"
#include "../graphics/shader/object_fwd.hpp"
#include "../physics/world_fwd.hpp"
#include "../physics/vec3.hpp"
#include "../input_delegator_fwd.hpp"
#include "../console/object_fwd.hpp"
#include <sge/systems/instance_fwd.hpp>
#include <boost/program_options/variables_map.hpp>

namespace insula
{
namespace vehicle
{
parameters const
cli_factory(
	boost::program_options::variables_map const &,
	scene::manager &,
	sge::systems::instance const &sys,
	graphics::camera::object &camera,
	graphics::shader::object &model_shader,
	physics::world &physics_world,
	physics::vec3 const &position,
	input_delegator &input_delegator_,
	console::object &console);
}
}

#endif
