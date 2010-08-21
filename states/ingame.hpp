#ifndef INSULA_STATES_INGAME_HPP_INCLUDED
#define INSULA_STATES_INGAME_HPP_INCLUDED

// _fwd isn't enough here
#include "../machine.hpp"
#include "../skydome/object_ptr.hpp"
#include "../skydome/console_proxy.hpp"
#include "../height_map/object_ptr.hpp"
#include "../height_map/console_proxy.hpp"
#include "../physics/world.hpp"
#include "../physics/height_map.hpp"
#include "../physics/debug_drawer.hpp"
#include "../water/object_ptr.hpp"
#include "../water/console_proxy.hpp"
// Why do these have to be completely defined? I thought forward
// declaring them was enough o_O
#include "../events/tick.hpp"
#include "../events/render.hpp"
#include <boost/statechart/state.hpp>
#include <boost/statechart/result.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/list/list10.hpp>

namespace insula
{
namespace states
{
/// The state represents "being in the game" as opposed to "being in
/// the menu" or something completely different. Here, the environment
/// for the current game (height field, physics world etc.) is
/// initialized.
class ingame
:
	public boost::statechart::state<ingame,machine>
{
public:
	typedef 
	boost::mpl::list2
	<
		boost::statechart::custom_reaction<events::tick>,
		boost::statechart::custom_reaction<events::render>
	> 
	reactions;

	explicit
	ingame(
		my_context);

	// Ticks the world
	boost::statechart::result
	react(
		events::tick const &);

	// Debug renders if requested
	boost::statechart::result
	react(
		events::render const &);

	// We've got pointers to forward-declared objects here
	~ingame();
private:
	// The heightmap has to be initialized first because it's needed for
	// the physics world
	height_map::object_ptr height_map_;
	height_map::console_proxy height_map_console_;
	physics::world physics_world_;
	physics::height_map physics_height_map_;
	physics::debug_drawer physics_debug_drawer_;
	bool physics_debug_;
	fcppt::signal::scoped_connection toggle_physics_debug_; 
	skydome::object_ptr skydome_;
	skydome::console_proxy skydome_console_;
	water::object_ptr water_;
	water::console_proxy water_console_;
};
}
}

#endif
