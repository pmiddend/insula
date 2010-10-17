#ifndef INSULA_MACHINE_HPP_INCLUDED
#define INSULA_MACHINE_HPP_INCLUDED

#include "states/game_outer_fwd.hpp"
#include "input_delegator.hpp"
#include "console/object.hpp"
#include "graphics/camera/object.hpp"
#include "graphics/stats.hpp"
#include "music_controller.hpp"
#include "sound_controller.hpp"
#include <sge/systems/instance.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/input/key_pair_fwd.hpp>
#include <sge/input/key_type_fwd.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/program_options/variables_map.hpp>

namespace insula
{
/// The machine should contain all information which is needed by all
/// substates, so no ingame information and nothing concerning the
/// menu (if existant later).
///
/// The machine also sends out tick events, although the ticks
/// themselves come from outside. I don't want to have an "infinite"
/// loop inside the machine, blocking everything else.
/// 
/// The complete state layout looks like this:
///
///         machine
///      /           |   
///    menu      game_outer
///                  |
///              game_inner
///        /         |           |          |         |         |
///   freelook   camera_move   pregame   running  finished   gameover
class machine
:
	public 
		boost::statechart::state_machine
		<
			machine,
			states::game_outer
		>
{
public:
	explicit
	machine(
		sge::parse::json::object const &config_file,
		boost::program_options::variables_map const &);

	// Sends events::tick, events::input and then events::render
	void
	tick(
		sge::time::timer::frames_type);

	boost::program_options::variables_map const &
	cli_variables() const;

	graphics::camera::object &
	camera();

	sge::systems::instance const &
	systems() const;

	insula::console::object &
	console();

	sge::parse::json::object const &
	config_file() const;

	music_controller &
	music();

	sound_controller &
	sounds();

	insula::input_delegator &
	input_delegator();

	// This is checked for in the main program (which _might_ exit if
	// it's false)
	bool 
	running() const;

	~machine();
private:
	bool running_;
	sge::parse::json::object config_file_;
	boost::program_options::variables_map vm_;
	sge::systems::instance systems_;
	insula::console::object console_;
	insula::input_delegator input_delegator_;
	// This is a ptr because the cli_factory returns a ptr
	graphics::camera::object camera_;
	fcppt::signal::scoped_connection exit_callback_;
	fcppt::signal::scoped_connection wireframe_callback_;
	fcppt::signal::scoped_connection 
		key_callback_,
		mouse_axis_callback_,
		mouse_button_callback_;
	graphics::stats stats_;
	bool show_stats_;
	fcppt::signal::scoped_connection stats_callback_;
	music_controller music_;
	sound_controller sounds_;

	void
	input_callback(
		sge::input::key_pair const &);

	void
	input_repeat_callback(
		sge::input::key_type const &);
};
}

#endif
