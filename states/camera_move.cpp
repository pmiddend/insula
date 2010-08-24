#include "camera_move.hpp"
#include "../events/tick.hpp"
#include "../events/render.hpp"

insula::states::camera_move::camera_move(
	my_context ctx)
:
	my_base(
		ctx)
{
}

boost::statechart::result
insula::states::camera_move::react(
	events::tick const &t)
{
	context<game_outer>().react(
		t); 
	context<game_inner>().react(
		t);

	return discard_event();
}

boost::statechart::result
insula::states::camera_move::react(
	events::render const &r)
{
	context<game_outer>().react(
		r);

	context<game_inner>().react(
		r);

	return discard_event();
}
