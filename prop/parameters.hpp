#ifndef INSULA_PROP_PARAMETERS_HPP_INCLUDED
#define INSULA_PROP_PARAMETERS_HPP_INCLUDED

#include "../graphics/camera/object_fwd.hpp"
#include "../graphics/shader/object_fwd.hpp"
#include "../height_map/object_fwd.hpp"
#include "../graphics/scalar.hpp"
#include "../physics/broadphase/manager_fwd.hpp"
#include <sge/parse/json/object.hpp>
#include <sge/systems/instance_fwd.hpp>

namespace insula
{
namespace prop
{
class parameters
{
public:
	sge::parse::json::object const &json_object;
	sge::systems::instance const &systems;
	graphics::camera::object &camera;
	graphics::shader::object &model_shader;
	graphics::shader::object &model_shadow_shader;
	insula::height_map::object &height_map;
	graphics::scalar water_level;
	scene::manager &scene_manager;
	physics::broadphase::manager &broadphase_manager;

	explicit
	parameters(
		sge::parse::json::object const &json_object,
		sge::systems::instance const &systems,
		graphics::camera::object &camera,
		graphics::shader::object &model_shader,
		graphics::shader::object &model_shadow_shader,
		insula::height_map::object &height_map,
		graphics::scalar const water_level,
		scene::manager &scene_manager,
		physics::broadphase::manager &broadphase_manager)
	:
		json_object(json_object),
		systems(systems),
		camera(camera),
		model_shader(model_shader),
		model_shadow_shader(model_shadow_shader),
		height_map(height_map),
		water_level(water_level),
		scene_manager(scene_manager),
		broadphase_manager(broadphase_manager)
	{
	}
};
}
}

#endif
