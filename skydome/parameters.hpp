#ifndef INSULA_SKYDOME_PARAMETERS_HPP_INCLUDED
#define INSULA_SKYDOME_PARAMETERS_HPP_INCLUDED

#include "../graphics/camera/object_fwd.hpp"
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

	explicit
	parameters(
		sge::parse::json::object const &config_file,
		graphics::camera::object const &camera,
		sge::systems::instance const &systems)
	:
		config_file(config_file),
		camera(camera),
		systems(systems)
	{
	}
};
}
}

#endif
