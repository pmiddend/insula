#ifndef INSULA_ARROW_PARAMETERS_HPP_INCLUDED
#define INSULA_ARROW_PARAMETERS_HPP_INCLUDED

#include "graphics/shader/object_fwd.hpp"
#include "physics/vec3.hpp"
#include <sge/systems/instance_fwd.hpp>
#include <sge/parse/json/object.hpp>
#include <functional>

namespace insula
{
class arrow_parameters
{
public:
	sge::systems::instance const &systems;
	sge::parse::json::object json;
	graphics::shader::object &model_shader;
	graphics::camera::object &camera;
	std::function<physics::vec3()> callback;

	explicit
	arrow_parameters(
		sge::systems::instance const &systems,
		sge::parse::json::object const &json,
		graphics::shader::object &model_shader,
		graphics::camera::object &camera,
		std::function<physics::vec3()> const &callback)
	:
		systems(systems),
		json(json),
		model_shader(model_shader),
		camera(camera),
		callback(callback)
	{
	}
};
}

#endif
