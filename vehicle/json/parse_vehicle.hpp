#ifndef INSULA_VEHICLE_JSON_PARSE_VEHICLE_HPP_INCLUDED
#define INSULA_VEHICLE_JSON_PARSE_VEHICLE_HPP_INCLUDED

#include "../parameters.hpp"
#include "../../graphics/camera/object_fwd.hpp"
#include "../../graphics/shader/object_fwd.hpp"
#include "../../graphics/scalar.hpp"
#include "../../physics/world_fwd.hpp"
#include "../../physics/vec3.hpp"
#include "../../console/object_fwd.hpp"
#include "../../input_delegator_fwd.hpp"
#include <sge/parse/json/object_fwd.hpp>
#include <sge/systems/instance_fwd.hpp>

namespace insula
{
namespace vehicle
{
namespace json
{
parameters const
parse_vehicle(
	sge::parse::json::object const &,
	// Passing this is _much_ easier than passing all the systems
	// separate
	sge::systems::instance const &,
	graphics::camera::object &,
	graphics::shader::object &,
	physics::world &,
	physics::vec3 const &position,
	input_delegator &,
	console::object &,
	graphics::scalar camera_distance,
	graphics::scalar camera_angle);
}
}
}

#endif
