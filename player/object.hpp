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
#include <sge/input/keyboard/key_event_fwd.hpp>
#include <sge/input/mouse/button_event_fwd.hpp>
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
	fcppt::signal::scoped_connection key_callback_,mouse_button_callback_;
	// Tells me if the given action should be executed currently
	action_map action_execute_;
	projectile::manager &projectiles_;

	void
	key_callback(
		sge::input::keyboard::key_event const &);

	void
	mouse_button_callback(
		sge::input::mouse::button_event const &);
};
}
}

#endif
