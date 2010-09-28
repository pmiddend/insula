#ifndef INSULA_STATES_FREELOOK_HPP_INCLUDED
#define INSULA_STATES_FREELOOK_HPP_INCLUDED

// _fwd is not enough
#include "game_inner.hpp"
#include "../events/tick.hpp"
#include "../events/render.hpp"
#include "../events/key.hpp"
#include "../model/backend.hpp"
#include "../model/instance.hpp"
#include <boost/statechart/state.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/list/list10.hpp>

namespace insula
{
namespace states
{
class freelook
:
	public boost::statechart::state<freelook,game_inner>
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
	freelook(
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

	~freelook();
private:
	model::backend player_backend_;
	model::instance player_instance_;
};
}
}

#endif
