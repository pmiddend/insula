#ifndef INSULA_PLAYER_PARAMETERS_HPP_INCLUDED
#define INSULA_PLAYER_PARAMETERS_HPP_INCLUDED

#include "../random_engine.hpp"
#include "../height_map/object_fwd.hpp"
#include "../height_map/scalar.hpp"
#include "../input_delegator_fwd.hpp"
#include "../graphics/camera/object_fwd.hpp"
#include "../physics/world_fwd.hpp"
#include <sge/parse/json/object.hpp>

namespace insula
{
namespace player
{
class parameters
{
public:
	sge::parse::json::object const &config_file;
	height_map::object &height_map;
	height_map::scalar &water_level;
	random_engine &rng_engine;
	graphics::camera::object &camera;
	insula::input_delegator &input_delegator;
	physics::world &physics_world;

	explicit
	parameters(
		sge::parse::json::object const &config_file,
		height_map::object &height_map,
		height_map::scalar water_level,
		random_engine &rng_engine,
		graphics::camera::object &camera,
		insula::input_delegator &input_delegator,
		physics::world &physics_world)
	:
		config_file(config_file),
		height_map(height_map),
		water_level(water_level),
		rng_engine(rng_engine),
		camera(camera),
		input_delegator(input_delegator),
		physics_world(physics_world)
	{
	}
};
}
}

#endif
