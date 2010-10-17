#include "pregame.hpp"
#include "camera_move.hpp"
#include "running.hpp"
#include "../events/tick.hpp"
#include "../events/render.hpp"
#include "../events/key.hpp"
#include "../sound_controller.hpp"
#include "../milliseconds_to_string.hpp"
#include <sge/font/draw_text.hpp>
#include <sge/font/text_part.hpp>
#include <sge/font/align_h.hpp>
#include <sge/font/align_v.hpp>
#include <sge/font/flags_none.hpp>
#include <sge/input/key_pair.hpp>
#include <sge/input/key_code.hpp>
#include <sge/renderer/device.hpp>
#include <sge/font/pos.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/int_type.hpp>
#include <fcppt/text.hpp>

insula::states::pregame::pregame(
	my_context ctx)
:
	my_base(
		ctx),
	remaining_time_(
		static_cast<std::chrono::milliseconds::rep>(
			sge::parse::json::find_member_exn<sge::parse::json::int_type>(
				context<machine>().config_file().members,
				FCPPT_TEXT("remaining-time"))))
{
	context<machine>().sounds().play(
		FCPPT_TEXT("plop"));
}

boost::statechart::result
insula::states::pregame::react(
	events::tick const &t)
{
	context<game_outer>().react(
		t); 

	context<game_inner>().react(
		t);

	return discard_event();
}

boost::statechart::result
insula::states::pregame::react(
	events::render const &r)
{
	context<game_outer>().react(
		r);

	context<game_inner>().react(
		r);

	sge::font::draw_text(
		context<game_outer>().large_font(),
		context<game_outer>().font_drawer(),
		FCPPT_TEXT("Player ")+
		context<game_inner>().current_player_name()+
		FCPPT_TEXT(", get ready\nPress return to continue"),
		sge::font::pos::null(),
		fcppt::math::dim::structure_cast<sge::font::dim>(
			context<machine>().systems().renderer()->screen_size()),
		sge::font::align_h::center,
		sge::font::align_v::center,
		sge::font::flags::none);

	sge::font::draw_text(
		context<game_outer>().large_font(),
		context<game_outer>().font_drawer(),
		milliseconds_to_string(
			remaining_time_),
		sge::font::pos::null(),
		sge::font::dim(
			static_cast<sge::font::unit>(
				context<machine>().systems().renderer()->screen_size().w()),
			static_cast<sge::font::unit>(
				static_cast<sge::font::unit>(
					context<machine>().systems().renderer()->screen_size().h())/
				static_cast<sge::font::unit>(
					6))),
		sge::font::align_h::center,
		sge::font::align_v::center,
		sge::font::flags::none);

	return discard_event();
}

boost::statechart::result
insula::states::pregame::react(
	events::key const &r)
{
	if (r.event().key().code() == sge::input::keyboard::key_code::return_)
	{
		fcppt::io::cout << FCPPT_TEXT("Return pressed, now switching to running\n");
		return 
			transit<running>(
				remaining_time_);
	}

	return discard_event();
}
