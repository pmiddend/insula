#ifndef INSULA_STATES_RUNNING_HPP_INCLUDED
#define INSULA_STATES_RUNNING_HPP_INCLUDED

// _fwd is not enough
#include "game_inner.hpp"
#include "../turn_timer.hpp"
#include "../events/tick.hpp"
#include "../events/render.hpp"
#include "../events/timer_expired.hpp"
#include "../events/key.hpp"
#include "../arrow.hpp"
#include <fcppt/signal/scoped_connection.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/list/list10.hpp>
#include <chrono>

namespace insula
{
namespace states
{
class running
:
	public boost::statechart::state<running,game_inner>
{
public:
	typedef 
	boost::mpl::list4
	<
		boost::statechart::custom_reaction<events::tick>,
		boost::statechart::custom_reaction<events::render>,
		boost::statechart::custom_reaction<events::timer_expired>,
		boost::statechart::custom_reaction<events::key>
	> 
	reactions;

	explicit
	running(
		my_context,
		std::chrono::milliseconds remaining_time);

	boost::statechart::result
	react(
		events::tick const &);

	boost::statechart::result
	react(
		events::render const &);

	boost::statechart::result
	react(
		events::timer_expired const &);

	boost::statechart::result
	react(
		events::key const &);
private:
	arrow arrow_;
	turn_timer timer_;
	fcppt::signal::scoped_connection nugget_callback_;

	void
	nugget_callback(
		physics::rigid::object &,
		physics::rigid::object &);
};
}
}

#endif
