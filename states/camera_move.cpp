#include "camera_move.hpp"
#include "pregame.hpp"
#include "../sound_controller.hpp"
#include "../events/tick.hpp"
#include "../graphics/scalar.hpp"
#include "../json/find_member.hpp"
#include "../graphics/camera/object.hpp"
#include "../gizmo/orthogonalize_simple.hpp"
#include "../gizmo/orthogonalize_keep_axis.hpp"
#include "../events/render.hpp"
#include "../player/object.hpp"
#include <fcppt/io/cout.hpp>
#include <boost/program_options/value_semantic.hpp>

insula::states::camera_move::camera_move(
	my_context ctx)
:
	my_base(
		ctx),
	pan_(
		context<machine>().camera().gizmo(),
		context<game_inner>().player().gizmo(),
		json::find_member<graphics::scalar>(
			context<machine>().config_file(),
			FCPPT_TEXT("camera-move/inverse-speed")),
		json::find_member<graphics::scalar>(
			context<machine>().config_file(),
			FCPPT_TEXT("camera-move/threshold")))
{
	context<machine>().camera().movement(
		false);
	context<machine>().sounds().play(
		FCPPT_TEXT("camera_swoosh"));
}

boost::statechart::result
insula::states::camera_move::react(
	events::tick const &t)
{
	context<game_outer>().react(
		t); 
	context<game_inner>().react(
		t);

	pan_.update(
		t.delta());

	context<machine>().camera().gizmo() = 
		pan_.current();

	if(pan_.finished())
	{
		fcppt::io::cout << "Panning is finished, now switching to pregame\n";
		return transit<pregame>();
	}

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
