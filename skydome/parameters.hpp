#ifndef INSULA_SKYDOME_PARAMETERS_HPP_INCLUDED
#define INSULA_SKYDOME_PARAMETERS_HPP_INCLUDED

#include "../graphics/camera/object_fwd.hpp"
#include "../scene/manager_fwd.hpp"
#include "../shadow/object_fwd.hpp"
#include <sge/parse/json/object_fwd.hpp>
#include <sge/systems/instance_fwd.hpp>

namespace insula
{
namespace skydome
{
class parameters
{
public:
	sge::parse::json::object const &config_file;
	graphics::camera::object const &camera;
	sge::systems::instance const &systems;
	scene::manager &scene_manager;
	// Can't be const since we want to register a callback here
	shadow::object &shadow;

	explicit
	parameters(
		sge::parse::json::object const &config_file,
		graphics::camera::object const &camera,
		sge::systems::instance const &systems,
		scene::manager &scene_manager,
		shadow::object &shadow)
	:
		config_file(config_file),
		camera(camera),
		systems(systems),
		scene_manager(scene_manager),
		shadow(shadow)
	{
	}
};
}
}

#endif
