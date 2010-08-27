#include "running.hpp"
#include "../events/tick.hpp"
#include "../events/render.hpp"
#include "../events/key.hpp"
#include "../vehicle/object.hpp"
#include "../physics/world.hpp"
#include "../sound_controller.hpp"
#include <sge/font/draw_text.hpp>
#include <sge/font/text_part.hpp>
#include <sge/font/align_h.hpp>
#include <sge/font/align_v.hpp>
#include <sge/font/flags_none.hpp>
#include <sge/renderer/device.hpp>

insula::states::running::running(
	my_context ctx)
:
	my_base(ctx)
{
	context<machine>().sounds().play(
		FCPPT_TEXT("honk"));
}

boost::statechart::result
insula::states::running::react(
	events::tick const &t)
{
	context<game_outer>().react(
		t); 

	context<game_inner>().react(
		t);

	context<game_inner>().physics_world().update(
		t.delta());

	context<game_inner>().vehicle().update();

	context<game_inner>().vehicle().update_camera();

	return discard_event();
}

boost::statechart::result
insula::states::running::react(
	events::render const &r)
{
	context<game_outer>().react(
		r);

	context<game_inner>().react(
		r);

	sge::font::draw_text(
		context<game_outer>().large_font(),
		context<game_outer>().font_drawer(),
		context<game_inner>().turn_timer().string(),
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
insula::states::running::react(
	events::key const &)
{
	return discard_event();
}
