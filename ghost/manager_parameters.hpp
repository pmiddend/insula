#ifndef INSULA_GHOST_MANAGER_PARAMETERS_HPP_INCLUDED
#define INSULA_GHOST_MANAGER_PARAMETERS_HPP_INCLUDED

#include "../scene/manager_fwd.hpp"
#include "../physics/broadphase/manager_fwd.hpp"
#include "../graphics/camera/object_fwd.hpp"
#include "../graphics/shader/object_fwd.hpp"
#include "../height_map/object_fwd.hpp"
#include "../height_map/scalar.hpp"
#include <sge/parse/json/array_fwd.hpp>
#include <sge/systems/instance_fwd.hpp>

namespace insula
{
namespace ghost
{
class manager_parameters
{
public:
	scene::manager &scene_manager;
	physics::broadphase::manager &broadphase_manager;
	sge::parse::json::array const &array;
	sge::systems::instance const &systems;
	graphics::camera::object &camera;
	graphics::shader::object &model_shader;
	height_map::object &height_map;
	height_map::scalar water_level;

	explicit
	manager_parameters(
		scene::manager &scene_manager,
		physics::broadphase::manager &broadphase_manager,
		sge::parse::json::array const &array,
		sge::systems::instance const &systems,
		graphics::camera::object &camera,
		graphics::shader::object &model_shader,
		height_map::object &height_map,
		height_map::scalar water_level)
	:
		scene_manager(scene_manager),
		broadphase_manager(broadphase_manager),
		array(array),
		systems(systems),
		camera(camera),
		model_shader(model_shader),
		height_map(height_map),
		water_level(water_level)
	{
	}
};
}
}

#endif
