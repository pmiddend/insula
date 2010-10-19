#ifndef INSULA_STATES_GAME_OUTER_HPP_INCLUDED
#define INSULA_STATES_GAME_OUTER_HPP_INCLUDED

#include <boost/statechart/state.hpp>
// fwd isn't enough here
#include "../machine.hpp"
#include "../height_map/object.hpp"
#include "../skydome/object.hpp"
#include "../shadow/object.hpp"
#include "../scene/manager.hpp"
#include "../graphics/shader/shared_object_ptr.hpp"
#include "../graphics/shader/object.hpp"
#include "../physics/broadphase/manager.hpp"
#include "../overlay/object.hpp"
#include "../water/object.hpp"
// If this is omitted, insula.cpp complains about incomplete tick
// events (!?)
#include "../events/tick.hpp"
#include "../events/render.hpp"
#include <sge/font/metrics_ptr.hpp>
#include <sge/font/drawer_ptr.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include "game_inner_fwd.hpp"
#include "../prop/manager.hpp"
#include "../nugget/manager.hpp"
#include "../ghost/manager.hpp"
#include "../random_engine.hpp"

#include <fcppt/optional.hpp>
#include <unordered_map>
#include <chrono>
#include <boost/program_options/options_description.hpp>
#include <fcppt/signal/scoped_connection.hpp>

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
		fcppt::string,
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

	// The large font is used for the messages in the non-game states
	// (like freelook)
	sge::font::metrics_ptr const 
	large_font() const;

	sge::font::drawer_ptr const
	font_drawer() const;

	// This is called by the inner game state to get the "current"
	// player
	fcppt::string const 
	next_player() const;

	// This determines the switch finished -> gameover
	bool 
	players_left() const;

	void
	place_time(
		fcppt::string const &player_name,
		std::chrono::milliseconds const &);

	player_time_map const &
	player_times() const;

	graphics::shader::object &
	model_shader();

	graphics::shader::object &
	model_shadow_shader();

	scene::manager &
	scene_manager();

	prop::manager &
	prop_manager();

	physics::broadphase::manager &
	broadphase_manager();

	random_engine &
	player_position_rng();

	// The player needs this because of random_point
	graphics::scalar
	water_level() const;

	// This is here just to test the camera in freelook
	shadow::object const &
	shadow_object() const { return shadow_; }

	// Holds height_map, skydome and the water
	~game_outer();
private:
	// This is used for command line input
	typedef
	std::vector<fcppt::string>
	player_sequence;

	graphics::shader::shared_object_ptr model_shader_;
	graphics::shader::shared_object_ptr model_shadow_shader_;
	scene::manager scene_manager_;
	shadow::object shadow_;
	insula::height_map::object height_map_;
	skydome::object skydome_;
	water::object water_;
	// This could also be in the machine, it's arbitrarily placed here
	sge::font::metrics_ptr large_font_;
	sge::font::drawer_ptr font_drawer_;
	player_time_map player_times_; 
	physics::broadphase::manager broadphase_manager_;
	prop::manager prop_manager_;
	ghost::manager ghost_manager_;
	random_engine player_position_rng_;
	overlay::object overlay_;
};
}
}

#endif
