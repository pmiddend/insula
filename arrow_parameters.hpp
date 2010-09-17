#ifndef INSULA_ARROW_PARAMETERS_HPP_INCLUDED
#define INSULA_ARROW_PARAMETERS_HPP_INCLUDED

#include "nugget/instance_fwd.hpp"
#include "graphics/shader/object_fwd.hpp"
#include <sge/systems/instance_fwd.hpp>
#include <sge/parse/json/object.hpp>

namespace insula
{
class arrow_parameters
{
public:
	sge::systems::instance const &systems;
	sge::parse::json::object json;
	graphics::shader::object &model_shader;
	graphics::camera::object &camera;
	nugget::instance const &nuggets;

	explicit
	arrow_parameters(
		sge::systems::instance const &systems,
		sge::parse::json::object const &json,
		graphics::shader::object &model_shader,
		graphics::camera::object &camera,
		nugget::instance const &nuggets)
	:
		systems(systems),
		json(json),
		model_shader(model_shader),
		camera(camera),
		nuggets(nuggets)
	{
	}
};
}

#endif
