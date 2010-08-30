#include "gameover.hpp"
#include "../music_controller.hpp"
#include "../events/tick.hpp"
#include "../events/render.hpp"
#include <sge/font/draw_text.hpp>
#include <sge/font/text_part.hpp>
#include <sge/font/align_h.hpp>
#include <sge/font/align_v.hpp>
#include <sge/font/flags_none.hpp>
#include <sge/input/key_pair.hpp>
#include <sge/input/key_code.hpp>
#include <sge/renderer/device.hpp>
#include <sge/font/pos.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/text.hpp>
#include <cmath>

insula::states::gameover::gameover(
	my_context ctx)
:
	my_base(
		ctx)
{
	//context<machine>().music().play_event(
	//	FCPPT_TEXT("gameover"));
}

boost::statechart::result
insula::states::gameover::react(
	events::tick const &t)
{
	context<game_outer>().react(
		t); 
	context<game_inner>().react(
		t);

	return discard_event();
}

boost::statechart::result
insula::states::gameover::react(
	events::render const &r)
{
	context<game_outer>().react(
		r);

	context<game_inner>().react(
		r);

	sge::font::draw_text(
		context<game_outer>().large_font(),
		context<game_outer>().font_drawer(),
		FCPPT_TEXT("So long and thanks for all the fish"),
		sge::font::pos::null(),
		fcppt::math::dim::structure_cast<sge::font::dim>(
			context<machine>().systems().renderer()->screen_size()),
		sge::font::align_h::center,
		sge::font::align_v::center,
		sge::font::flags::none);

	return discard_event();
}

insula::states::gameover::~gameover()
{
}
