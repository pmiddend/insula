#ifndef INSULA_STATES_GAME_OUTER_HPP_INCLUDED
#define INSULA_STATES_GAME_OUTER_HPP_INCLUDED

#include <boost/statechart/state.hpp>
// fwd isn't enough here
#include "../machine.hpp"
#include "../height_map/object_ptr.hpp"
#include "../height_map/object_fwd.hpp"
#include "../height_map/console_proxy.hpp"
#include "../skydome/object_ptr.hpp"
#include "../skydome/console_proxy.hpp"
#include "../water/object_ptr.hpp"
#include "../water/console_proxy.hpp"
#include "../nugget_sequence.hpp"
// If this is omitted, insula.cpp complains about incomplete tick
// events (!?)
#include "../events/tick.hpp"
#include "../events/render.hpp"
#include "game_inner_fwd.hpp"

namespace insula
{
namespace states
{
class game_outer
:
	public boost::statechart::state<game_outer,machine,game_inner>
{
public:
	explicit
	game_outer(
		my_context);

	insula::height_map::object &
	height_map();

	insula::height_map::object const &
	height_map() const;

	void
	react(
		events::tick const &);

	void
	react(
		events::render const &);

	// Holds height_map, skydome and the water
	~game_outer();
private:
	insula::height_map::object_ptr height_map_;
	insula::height_map::console_proxy height_map_console_;
	skydome::object_ptr skydome_;
	skydome::console_proxy skydome_console_;
	insula::water::object_ptr water_;
	insula::water::console_proxy water_console_;
	nugget_sequence nugget_positions_;
};
}
}

#endif