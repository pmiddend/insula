#ifndef INSULA_STATES_GAMEOVER_HPP_INCLUDED
#define INSULA_STATES_GAMEOVER_HPP_INCLUDED

// _fwd is not enough
#include "game_inner.hpp"
#include "../events/tick.hpp"
#include "../events/render.hpp"
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/list/list10.hpp>

namespace insula
{
namespace states
{
class gameover
:
	public boost::statechart::state<gameover,game_inner>
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
	gameover(
		my_context);

	boost::statechart::result
	react(
		events::tick const &);

	boost::statechart::result
	react(
		events::render const &);

	~gameover();
};
}
}

#endif
