#ifndef INSULA_MACHINE_HPP_INCLUDED
#define INSULA_MACHINE_HPP_INCLUDED

#include "states/ingame_fwd.hpp"
#include "input_delegator.hpp"
#include "console/object.hpp"
#include "graphics/camera/object_ptr.hpp"
#include "graphics/stats.hpp"
#include "music_controller.hpp"
#include "sound_controller.hpp"
#include <sge/systems/instance.hpp>
#include <sge/parse/json/object.hpp>
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
class machine
:
	public 
		boost::statechart::state_machine
		<
			machine,
			states::ingame
		>
{
public:
	explicit
	machine(
		boost::program_options::variables_map const &);

	// Sends events::tick and then events::render
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
	input_delegator input_delegator_;
	// This is a ptr because the cli_factory returns a ptr
	graphics::camera::object_ptr camera_;
	fcppt::signal::scoped_connection exit_callback_;
	fcppt::signal::scoped_connection wireframe_callback_;
	graphics::stats stats_;
	bool show_stats_;
	fcppt::signal::scoped_connection stats_callback_;
	music_controller music_;
	sound_controller sounds_;
};
}

#endif
