#ifndef INSULA_SHADOW_PARAMETERS_HPP_INCLUDED
#define INSULA_SHADOW_PARAMETERS_HPP_INCLUDED

#include "../graphics/gizmo.hpp"
#include "../scene/manager_fwd.hpp"
#include <sge/systems/instance_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/math/dim/basic_impl.hpp>

namespace insula
{
namespace shadow
{
class parameters
{
public:
	sge::parse::json::object const &config_file;
	sge::systems::instance const &systems;
	scene::manager &scene_manager;

	explicit
	parameters(
		sge::parse::json::object const &config_file,
		sge::systems::instance const &systems,
		scene::manager &scene_manager)
	:
		config_file(config_file),
		systems(systems),
		scene_manager(scene_manager)
	{
	}
};
}
}

#endif
