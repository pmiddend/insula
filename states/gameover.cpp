#include "gameover.hpp"
#include "../music_controller.hpp"
#include "../events/tick.hpp"
#include "../events/render.hpp"
#include "../stdlib/map.hpp"
#include "../milliseconds_to_string.hpp"
#include <sge/font/draw_text.hpp>
#include <sge/font/text_part.hpp>
#include <sge/font/align_h.hpp>
#include <sge/font/align_v.hpp>
#include <sge/font/flags_none.hpp>
#include <sge/renderer/device.hpp>
#include <sge/font/pos.hpp>
#include <fcppt/text.hpp>
#include <fcppt/format.hpp>
#include <cmath>

insula::states::gameover::gameover(
	my_context ctx)
:
	my_base(
		ctx),
	times_(
		stdlib::map<time_table>(
			context<game_outer>().player_times(),
			[](game_outer::player_time_map::value_type const &v)
			{
				return time_table::value_type(*v.second,v.first);
			}))
{
	context<machine>().music().play_event(
		FCPPT_TEXT("gameover"));
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

	unsigned place = 1;
	sge::font::draw_text(
		context<game_outer>().large_font(),
		context<game_outer>().font_drawer(),
		std::accumulate(
			times_.begin(),
			times_.end(),
			fcppt::string(FCPPT_TEXT("Score board: \n")),
			[&place](
				fcppt::string const &s,
				time_table::value_type const &v)
			{
				return 
					(fcppt::format(FCPPT_TEXT("%s%i.: %i %s\n"))
						% s
						% (place++)
						% milliseconds_to_string(v.first)
						% v.second).str();
			}),
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
