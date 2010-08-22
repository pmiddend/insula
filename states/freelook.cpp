#include "freelook.hpp"
#include "../events/tick.hpp"
#include "../events/render.hpp"

insula::states::freelook::freelook(
	my_context ctx)
:
	my_base(
		ctx)
{
	//context<ingame>().regenerate_nuggets();
}

boost::statechart::result
insula::states::freelook::react(
	events::tick const &t)
{
	context<game_outer>().react(
		t);

	context<game_inner>().react(
		t);
}

boost::statechart::result
insula::states::freelook::react(
	events::render const &r)
{
	context<game_outer>().react(
		r);

	context<game_inner>().react(
		r);
}
