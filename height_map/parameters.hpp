#ifndef INSULA_HEIGHT_MAP_PARAMETERS_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_PARAMETERS_HPP_INCLUDED

#include "../graphics/camera/object_fwd.hpp"
#include "../graphics/mat4.hpp"
#include "../scene/manager_fwd.hpp"
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/image/multi_loader_fwd.hpp>
#include <sge/renderer/texture_ptr.hpp>

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
	sge::renderer::texture_ptr shadow_map;
	graphics::mat4 shadow_mvp;

	explicit
	parameters(
		sge::parse::json::object const &config_file,
		graphics::camera::object const &camera,
		sge::renderer::device_ptr renderer,
		sge::image::multi_loader &image_loader,
		scene::manager &scene_manager,
		sge::renderer::texture_ptr shadow_map,
		graphics::mat4 const &shadow_mvp)
	:
		config_file(config_file),
		camera(camera),
		renderer(renderer),
		image_loader(image_loader),
		scene_manager(scene_manager),
		shadow_map(shadow_map),
		shadow_mvp(shadow_mvp)
	{
	}
};
}
}

#endif
