#include "finished.hpp"
// Has to be here since we switch vom finished to game_inner which has
// freelook as its initial state
#include "freelook.hpp"
#include "camera_move.hpp"
#include "gameover.hpp"
#include "../music_controller.hpp"
#include "../sound_controller.hpp"
#include "../events/tick.hpp"
#include "../events/render.hpp"
#include "../gizmo/orthogonalize_simple.hpp"
#include "../gizmo/orthogonalize_keep_axis.hpp"
#include "../gizmo/lock_to.hpp"
#include "../height_map/object.hpp"
#include "../graphics/camera/object.hpp"
#include <sge/font/draw_text.hpp>
#include <sge/font/text_part.hpp>
#include <sge/font/align_h.hpp>
#include <sge/font/align_v.hpp>
#include <sge/font/flags_none.hpp>
#include <sge/input/key_pair.hpp>
#include <sge/input/key_code.hpp>
#include <sge/renderer/device.hpp>
#include <sge/font/pos.hpp>
#include <fcppt/math/pi.hpp>
#include <fcppt/text.hpp>
#include <cmath>

insula::states::finished::finished(
	my_context ctx)
:
	my_base(
		ctx)
{
	context<machine>().sounds().play(
		FCPPT_TEXT("clap"));
}

boost::statechart::result
insula::states::finished::react(
	events::tick const &t)
{
	context<game_outer>().react(
		t); 
	context<game_inner>().react(
		t);

	return discard_event();
}

boost::statechart::result
insula::states::finished::react(
	events::render const &r)
{
	context<game_outer>().react(
		r);

	context<game_inner>().react(
		r);

	fcppt::string const s = 
		context<game_outer>().players_left()
		?
			FCPPT_TEXT("Press enter for next player")
		:
			FCPPT_TEXT("Press enter for the score board");

	sge::font::draw_text(
		context<game_outer>().large_font(),
		context<game_outer>().font_drawer(),
		FCPPT_TEXT("Great job!\n")+s,
		sge::font::pos::null(),
		fcppt::math::dim::structure_cast<sge::font::dim>(
			context<machine>().systems().renderer()->screen_size()),
		sge::font::align_h::center,
		sge::font::align_v::center,
		sge::font::flags::none);

	return discard_event();
}

boost::statechart::result
insula::states::finished::react(
	events::key const &r)
{
	if (r.event().key().code() == sge::input::keyboard::key_code::return_)
	{
		if (context<game_outer>().players_left())
			return transit<game_inner>();
		return transit<gameover>();
	}

	return discard_event();
}

insula::states::finished::~finished()
{
	context<machine>().music().stop();
}
