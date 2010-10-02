#ifndef INSULA_STATES_PREGAME_HPP_INCLUDED
#define INSULA_STATES_PREGAME_HPP_INCLUDED

// _fwd is not enough
#include "game_inner.hpp"
#include "../events/tick.hpp"
#include "../events/render.hpp"
#include "../events/key.hpp"
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/list/list10.hpp>
#include <chrono>

namespace insula
{
namespace states
{
/**
	In this state, the camera is in the first person perspective and we
	are just waiting for the current player to press the start button.
 */
class pregame
:
	public boost::statechart::state<pregame,game_inner>
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
	pregame(
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
	std::chrono::milliseconds remaining_time_;
};
}
}

#endif
