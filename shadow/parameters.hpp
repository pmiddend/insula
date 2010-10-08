#ifndef INSULA_SHADOW_PARAMETERS_HPP_INCLUDED
#define INSULA_SHADOW_PARAMETERS_HPP_INCLUDED

#include "../graphics/gizmo.hpp"
#include "../scene/manager_fwd.hpp"
#include <sge/renderer/dim_type.hpp>
#include <sge/systems/instance_fwd.hpp>
#include <fcppt/math/dim/basic_impl.hpp>

namespace insula
{
namespace shadow
{
class parameters
{
public:
	sge::systems::instance const &systems;
	graphics::gizmo sun_gizmo;
	sge::renderer::dim_type texture_size;
	scene::manager &scene_manager;

	explicit
	parameters(
		sge::systems::instance const &systems,
		graphics::gizmo const &sun_gizmo,
		sge::renderer::dim_type const &texture_size,
		scene::manager &scene_manager)
	:
		systems(systems),
		sun_gizmo(sun_gizmo),
		texture_size(texture_size),
		scene_manager(scene_manager)
	{
	}
};
}
}

#endif
