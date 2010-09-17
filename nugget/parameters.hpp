#ifndef INSULA_NUGGET_PARAMETERS_HPP_INCLUDED
#define INSULA_NUGGET_PARAMETERS_HPP_INCLUDED

#include "../height_map/object_fwd.hpp"
#include "../graphics/scalar.hpp"
#include "../graphics/camera/object_fwd.hpp"
#include "../graphics/shader/object_fwd.hpp"
#include "../physics/world_fwd.hpp"
#include "../scene/manager_fwd.hpp"
#include "../sound_controller.hpp"
#include <sge/systems/instance_fwd.hpp>
#include <sge/parse/json/object.hpp>

namespace insula
{
namespace nugget
{
class parameters
{
public:
	sge::parse::json::object const &json;
	insula::height_map::object &height_map;
	graphics::scalar water_level;
	sound_controller &sounds;
	sge::systems::instance const &systems;
	graphics::camera::object &camera;
	graphics::shader::object &model_shader;
	physics::world &world;
	scene::manager &scene_manager;
	physics::broadphase::manager &broadphase_manager;
	
	explicit
	parameters(
		sge::parse::json::object const &json,
		insula::height_map::object &height_map,
		graphics::scalar water_level,
		sound_controller &sounds,
		sge::systems::instance const &systems,
		graphics::camera::object &camera,
		graphics::shader::object &model_shader,
		physics::world &world,
		scene::manager &scene_manager,
		physics::broadphase::manager &broadphase_manager)
	:
		json(json),
		height_map(height_map),
		water_level(water_level),
		sounds(sounds),
		systems(systems),
		camera(camera),
		model_shader(model_shader),
		world(world),
		scene_manager(scene_manager),
		broadphase_manager(broadphase_manager)
	{
	}
};
}
}

#endif
