#include "running.hpp"
#include "finished.hpp"
#include "../arrow_parameters.hpp"
#include "../timed_output.hpp"
#include "../events/tick.hpp"
#include "../events/render.hpp"
#include "../events/key.hpp"
#include "../vehicle/object.hpp"
#include "../physics/world.hpp"
#include "../sound_controller.hpp"
#include "../music_controller.hpp"
#include <sge/font/draw_text.hpp>
#include <sge/font/text_part.hpp>
#include <sge/font/align_h.hpp>
#include <sge/font/align_v.hpp>
#include <sge/font/flags_none.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/renderer/device.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/text.hpp>
#include <functional>

#include "../physics/height_map.hpp"
#include "../physics/vehicle/object.hpp"

insula::states::running::running(
	my_context ctx)
:
	my_base(
		ctx),
	scoped_vehicle_(
		context<game_inner>().vehicle()),
	arrow_(
		arrow_parameters(
			context<machine>().systems(),
			sge::parse::json::find_member_exn<sge::parse::json::object>(
				context<machine>().config_file().members,
				FCPPT_TEXT("arrow")),
			context<game_outer>().model_shader(),
			context<machine>().camera(),
			context<game_inner>().nugget_instance())),
	vehicle_crash_connection_(
		context<game_inner>().physics_world().register_callback
		<
			physics::vehicle::object,
			physics::height_map
		>(
			physics::object_type::vehicle,
			physics::object_type::height_map,
			[this](physics::vehicle::object &,physics::height_map &)
			{
				this->context<machine>().sounds().play(FCPPT_TEXT("crash"));
			}))
{
	context<machine>().sounds().play(
		FCPPT_TEXT("honk"));

	//context<machine>().music().play_random();

	context<game_inner>().turn_timer().start();
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

	context<game_inner>().vehicle().update(
		t.delta());

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

	arrow_.render();

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

	context<game_inner>().vehicle().render();

	return discard_event();
}

boost::statechart::result
insula::states::running::react(
	events::nuggets_empty const &)
{
	context<game_inner>().turn_timer().stop();
	context<game_outer>().place_time(
		context<game_inner>().current_player(),
	context<game_inner>().turn_timer().milliseconds());
	return transit<finished>();
}

boost::statechart::result
insula::states::running::react(
	events::key const &)
{
	return discard_event();
}
