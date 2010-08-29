#ifndef INSULA_STATES_RUNNING_HPP_INCLUDED
#define INSULA_STATES_RUNNING_HPP_INCLUDED

// _fwd is not enough
#include "game_inner.hpp"
#include "../events/tick.hpp"
#include "../events/render.hpp"
#include "../events/key.hpp"
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/list/list10.hpp>

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
	boost::mpl::list3
	<
		boost::statechart::custom_reaction<events::tick>,
		boost::statechart::custom_reaction<events::render>,
		boost::statechart::custom_reaction<events::key>
	> 
	reactions;

	explicit
	running(
		my_context);

	boost::statechart::result
	react(
		events::tick const &);

	boost::statechart::result
	react(
		events::render const &);

	boost::statechart::result
	react(
		events::key const &);
private:
};
}
}

#endif