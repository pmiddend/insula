#ifndef INSULA_PROP_PARAMETERS_HPP_INCLUDED
#define INSULA_PROP_PARAMETERS_HPP_INCLUDED

#include "../graphics/camera/object_fwd.hpp"
#include "../graphics/shader/object_fwd.hpp"
#include "../height_map/object_fwd.hpp"
#include "../graphics/scalar.hpp"
#include <sge/parse/json/object.hpp>
#include <sge/systems/instance_fwd.hpp>

namespace insula
{
namespace prop
{
class parameters
{
public:
	sge::parse::json::object json_object;
	sge::systems::instance const &systems;
	graphics::camera::object &camera;
	graphics::shader::object &model_shader;
	insula::height_map::object &height_map;
	graphics::scalar water_level;

	explicit
	parameters(
		sge::parse::json::object json_object,
		sge::systems::instance const &systems,
		graphics::camera::object &camera,
		graphics::shader::object &model_shader,
		insula::height_map::object &height_map,
		graphics::scalar water_level)
	:
		json_object(json_object),
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
