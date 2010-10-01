#ifndef INSULA_PLAYER_OBJECT_HPP_INCLUDED
#define INSULA_PLAYER_OBJECT_HPP_INCLUDED

#include "parameters_fwd.hpp"
#include "action.hpp"
#include "../physics/character_controller.hpp"
#include "../physics/gizmo.hpp"
#include "../physics/scalar.hpp"
#include "../time_delta.hpp"
#include "../graphics/camera/object_fwd.hpp"
#include "../projectile/manager_fwd.hpp"
#include <sge/input/key_pair_fwd.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <map>

namespace insula
{
namespace player
{
class object
{
public:
	object(object const &) = delete;
	object &operator=(object const &) = delete;

	explicit
	object(
		parameters const &);

	physics::gizmo const
	gizmo() const;

	void
	update(
		time_delta);
private:
	typedef
	std::map
	<
		action::type,
		bool
	>
	action_map;

	graphics::camera::object &camera_;
	physics::character_controller character_controller_;
	physics::scalar walk_speed_;
	fcppt::signal::scoped_connection input_connection_;
	// Tells me if the given action should be executed currently
	action_map action_execute_;
	projectile::manager &projectiles_;

	void
	input_callback(
		sge::input::key_pair const &);
};
}
}

#endif
