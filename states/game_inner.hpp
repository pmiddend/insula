#ifndef INSULA_STATES_GAME_INNER_HPP_INCLUDED
#define INSULA_STATES_GAME_INNER_HPP_INCLUDED

#include "game_outer.hpp"
#include "freelook_fwd.hpp"
#include "../static_model_instance.hpp"
#include "../player/object.hpp"
#include "../prop/instance_sequence.hpp"
#include "../physics/world.hpp"
#include "../physics/height_map.hpp"
#include "../physics/debug_drawer.hpp"
#include "../physics/broadphase/manager.hpp"
#include "../events/tick_fwd.hpp"
#include "../events/render_fwd.hpp"
#include <fcppt/signal/scoped_connection.hpp>
#include <boost/statechart/state.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace insula
{
namespace states
{
class game_inner
:
	public boost::statechart::state<game_inner,game_outer,freelook>
{
public:
	explicit
	game_inner(
		my_context);

	void
	react(
		events::tick const &);

	void
	react(
		events::render const &);
	
	fcppt::string const &
	current_player_name() const;

	physics::world &
	physics_world();

/*
	nugget::instance &
	nugget_instance();

	nugget::instance const &
	nugget_instance() const;
*/

	player::object &
	player();

	player::object const &
	player() const;

	~game_inner();
private:
	fcppt::string current_player_name_;
	physics::world physics_world_;
	physics::height_map physics_height_map_;
	physics::debug_drawer physics_debug_drawer_;
	bool physics_debug_;
	fcppt::signal::scoped_connection toggle_physics_debug_; 
	prop::shared_instance_ptr props_;
	player::object player_;
};
}
}

#endif
