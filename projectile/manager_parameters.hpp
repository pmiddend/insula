#ifndef INSULA_PROJECTILE_MANAGER_PARAMETERS_HPP_INCLUDED
#define INSULA_PROJECTILE_MANAGER_PARAMETERS_HPP_INCLUDED

#include "../graphics/camera/object_fwd.hpp"
#include "../graphics/shader/object_fwd.hpp"
#include "../scene/manager_fwd.hpp"
#include "../physics/broadphase/manager_fwd.hpp"
#include "../physics/world_fwd.hpp"
#include <sge/parse/json/object.hpp>
#include <sge/systems/instance_fwd.hpp>

namespace insula
{
namespace projectile
{
class manager_parameters
{
public:
	sge::parse::json::object const &config_file;
	sge::systems::instance const &systems;
	graphics::camera::object const &camera;
	graphics::shader::object &model_shader;
	graphics::shader::object &model_shadow_shader;
	scene::manager &scene_manager;
	physics::broadphase::manager &broadphase;
	physics::world &world;

	explicit
	manager_parameters(
		sge::parse::json::object const &config_file,
		sge::systems::instance const &systems,
		graphics::camera::object const &camera,
		graphics::shader::object &model_shader,
		graphics::shader::object &model_shadow_shader,
		scene::manager &scene_manager,
		physics::broadphase::manager &broadphase,
		physics::world &world)
	:
		config_file(config_file),
		systems(systems),
		camera(camera),
		model_shader(model_shader),
		model_shadow_shader(model_shadow_shader),
		scene_manager(scene_manager),
		broadphase(broadphase),
		world(world)
	{
	}
};
}
}

#endif
