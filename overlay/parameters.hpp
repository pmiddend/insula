#ifndef INSULA_OVERLAY_PARAMETERS_HPP_INCLUDED
#define INSULA_OVERLAY_PARAMETERS_HPP_INCLUDED

#include "../scene/manager_fwd.hpp"
#include "../graphics/camera/object_fwd.hpp"
#include <sge/systems/instance_fwd.hpp>

namespace insula
{
namespace overlay
{
class parameters
{
public:
	sge::systems::instance const &systems;
	scene::manager &scene_manager;
	// We need the camera for the perspective matrix (to get the near
	// plane) and also to scope it and set the mvp matrix
	graphics::camera::object &camera;

	explicit
	parameters(
		sge::systems::instance const &systems,
		scene::manager &scene_manager,
		graphics::camera::object &camera)
	:
		systems(systems),
		scene_manager(scene_manager),
		camera(camera)
	{
	}
};
}
}

#endif
