#ifndef INSULA_WATER_PARAMETERS_HPP_INCLUDED
#define INSULA_WATER_PARAMETERS_HPP_INCLUDED

#include <sge/parse/json/object_fwd.hpp>
#include <sge/systems/instance_fwd.hpp>
#include "../graphics/camera/object_fwd.hpp"
#include "../graphics/box.hpp"
#include "../scene/manager_fwd.hpp"

namespace insula
{
namespace water
{
class parameters
{
public:
	sge::parse::json::object const &config_file;
	sge::systems::instance const &systems;
	graphics::camera::object &camera;
	graphics::box const &extents;
	scene::manager &scene_manager;

	explicit
	parameters(
		sge::parse::json::object const &config_file,
		sge::systems::instance const &systems,
		graphics::camera::object &camera,
		graphics::box const &extents,
		scene::manager &scene_manager)
	:
		config_file(config_file),
		systems(systems),
		camera(camera),
		extents(extents),
		scene_manager(scene_manager)
	{
	}
};
}
}

#endif
