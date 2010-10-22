#ifndef INSULA_HEIGHT_MAP_PARAMETERS_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_PARAMETERS_HPP_INCLUDED

#include "../graphics/camera/object_fwd.hpp"
#include "../scene/manager_fwd.hpp"
#include "../shadow/object_fwd.hpp"
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/image/multi_loader_fwd.hpp>

namespace insula
{
namespace height_map
{
class parameters
{
public:
	sge::parse::json::object const &config_file;
	graphics::camera::object const &camera;
	sge::renderer::device_ptr renderer;
	sge::image::multi_loader &image_loader;
	scene::manager &scene_manager;
	// This could be const, but the shadow::object has a "texture_ptr
	// texture()" member function which isn't const
	shadow::object &shadow;

	explicit
	parameters(
		sge::parse::json::object const &config_file,
		graphics::camera::object const &camera,
		sge::renderer::device_ptr renderer,
		sge::image::multi_loader &image_loader,
		scene::manager &scene_manager,
		shadow::object &shadow)
	:
		config_file(config_file),
		camera(camera),
		renderer(renderer),
		image_loader(image_loader),
		scene_manager(scene_manager),
		shadow(shadow)
	{
	}
};
}
}

#endif
