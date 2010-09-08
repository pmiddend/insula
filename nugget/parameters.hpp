#ifndef INSULA_NUGGET_PARAMETERS_HPP_INCLUDED
#define INSULA_NUGGET_PARAMETERS_HPP_INCLUDED

#include "../height_map/object_fwd.hpp"
#include "../graphics/scalar.hpp"
#include "../graphics/camera/object_fwd.hpp"
#include "../graphics/shader/object_fwd.hpp"
#include "../physics/world_fwd.hpp"
#include "../scene/manager_fwd.hpp"
#include <sge/systems/instance_fwd.hpp>

namespace insula
{
namespace nugget
{
class parameters
{
public:
	insula::height_map::object &height_map;
	graphics::scalar water_level;
	sge::systems::instance const &systems;
	graphics::camera::object &camera;
	graphics::shader::object &model_shader;
	physics::world &world;
	scene::manager &scene_manager;
	
	explicit
	parameters(
		insula::height_map::object &height_map,
		graphics::scalar water_level,
		sound_controller &sounds,
		sge::systems::instance const &systems,
		graphics::camera::object &camera,
		graphics::shader::object &model_shader)
	:
		height_map(height_map),
		water_level(water_level),
		sounds(sounds),
		systems(systems),
		camera(camera),
		model_shader(model_shader)
	{
	}
};
}
}

#endif
