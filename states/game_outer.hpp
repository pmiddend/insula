#ifndef INSULA_STATES_GAME_OUTER_HPP_INCLUDED
#define INSULA_STATES_GAME_OUTER_HPP_INCLUDED

#include <boost/statechart/state.hpp>
// fwd isn't enough here
#include "../machine.hpp"
#include "../player.hpp"
#include "../height_map/object_ptr.hpp"
#include "../height_map/object_fwd.hpp"
#include "../skydome/object_ptr.hpp"
#include "../water/object_ptr.hpp"
#include "../scene/manager.hpp"
#include "../graphics/shader/object.hpp"
#include "../physics/broadphase/manager.hpp"
#include "../nugget_sequence.hpp"
// If this is omitted, insula.cpp complains about incomplete tick
// events (!?)
#include "../events/tick.hpp"
#include "../events/render.hpp"
#include <sge/font/metrics_ptr.hpp>
#include <sge/font/drawer_ptr.hpp>
#include "game_inner_fwd.hpp"
#include "../prop/manager.hpp"
#include "../nugget/manager.hpp"
#include "../ghost/manager.hpp"

// vehicle position begin
#include "../random_engine.hpp"
#include "../graphics/vec2.hpp"
#include <fcppt/math/vector/basic_impl.hpp>
// vehicle position end

// playermap begin
#include <fcppt/optional.hpp>
#include <unordered_map>
#include <chrono>
#include <boost/program_options/options_description.hpp>
// playermap end

namespace insula
{
namespace states
{
class game_outer
:
	public boost::statechart::state<game_outer,machine,game_inner>
{
public:
	// std::map doesn't perform very well with strings as keys. This has
	// to be publicly available because states::gameover needs it to
	// build its table.
	typedef
	std::unordered_map
	<
		player,
		fcppt::optional<std::chrono::milliseconds>
	>
	player_time_map;

	explicit
	game_outer(
		my_context);

	insula::height_map::object &
	height_map();

	insula::height_map::object const &
	height_map() const;

	// This is called "react" so it can be easily converted to an event
	// reaction function (which it originally was)
	void
	react(
		events::tick const &);

	void
	react(
		events::render const &);

	// The nugget positions don't change in each iteration, so read-only
	// access here
	nugget_sequence const &
	nugget_positions() const;

	graphics::vec2 const &
	vehicle_position() const;

	// The large font is used for the messages in the non-game states
	// (like freelook)
	sge::font::metrics_ptr const 
	large_font() const;

	sge::font::drawer_ptr const

	font_drawer() const;

	// This is called by the inner game state to get the "current"
	// player
	player const 
	next_player() const;

	// This determines the switch finished -> gameover
	bool 
	players_left() const;

	void
	place_time(
		player const &,
		std::chrono::milliseconds const &);

	player_time_map const &
	player_times() const;

	graphics::shader::object &
	model_shader();

	scene::manager &
	scene_manager();

	nugget::manager &
	nugget_manager();

	prop::manager &
	prop_manager();

	// Holds height_map, skydome and the water
	~game_outer();

	static
	boost::program_options::options_description const
	cli_options();
private:
	// This is used for command line input
	typedef
	std::vector<player>
	player_sequence;

	graphics::shader::object model_shader_;
	scene::manager scene_manager_;
	insula::height_map::object_ptr height_map_;
	skydome::object_ptr skydome_;
	insula::water::object_ptr water_;
	nugget_sequence nugget_positions_;
	random_engine vehicle_position_engine_;
	graphics::vec2 const vehicle_position_;
	// This could also be in the machine, it's arbitrarily placed here
	sge::font::metrics_ptr large_font_;
	sge::font::drawer_ptr font_drawer_;
	player_time_map player_times_; 
	physics::broadphase::manager broadphase_manager_;
	nugget::manager nugget_manager_;
	prop::manager prop_manager_;
	ghost::manager ghost_manager_;
};
}
}

#endif
