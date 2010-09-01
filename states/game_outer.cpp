#include "game_outer.hpp"
#include "../random_seed.hpp"
#include "../random_flat_point.hpp"
#include "../generate_nuggets.hpp"
#include "../create_path.hpp"
#include "../get_option.hpp"
#include "../height_map/cli_factory.hpp"
#include "../stdlib/map.hpp"
#include "../height_map/object.hpp"
#include "../skydome/cli_factory.hpp"
#include "../skydome/object.hpp"
#include "../water/cli_factory.hpp"
#include "../water/object.hpp"
#include "../json/parse_font.hpp"
#include "../exception.hpp"
#include <fcppt/math/box/structure_cast.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/text.hpp>
#include <sge/renderer/state/cull_mode.hpp>
#include <sge/font/drawer_3d.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/string.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/color/rgb8.hpp>
#include <mizuiro/color/init.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <algorithm>

insula::states::game_outer::game_outer(
	my_context ctx)
:
	my_base(
		ctx),
	height_map_(
		insula::height_map::cli_factory(
			context<machine>().cli_variables(),
			context<machine>().camera(),
			context<machine>().systems().renderer(),
			context<machine>().systems().image_loader())),
	skydome_(
		skydome::cli_factory(
			context<machine>().cli_variables(),
			context<machine>().camera(),
			context<machine>().systems().renderer())),
	water_(
		insula::water::cli_factory(
			context<machine>().cli_variables(),
			context<machine>().systems().renderer(),
			context<machine>().camera(),
			fcppt::math::box::structure_cast<graphics::box>(
				height_map_->extents()),
			context<machine>().systems().image_loader())),
	nugget_positions_(
		generate_nuggets(
			*height_map_,
			water_->water_level(),
			get_option<nugget_sequence::size_type>(
				context<machine>().cli_variables(),
				"game-nuggets"))),
	vehicle_position_engine_(
		random_seed()),
	vehicle_position_(
		random_flat_point(
			*height_map_,
			water_->water_level(),
			vehicle_position_engine_)),
	large_font_(
		json::parse_font(
			sge::parse::json::find_member_exn<sge::parse::json::object>(
				context<machine>().config_file().members,
				FCPPT_TEXT("large_font")),
			context<machine>().systems().font_system())),
	font_drawer_(
		fcppt::make_shared_ptr<sge::font::drawer_3d>(
			context<machine>().systems().renderer(),
			sge::image::color::rgb8(
				(mizuiro::color::init::red %= 1.0)
				(mizuiro::color::init::green %= 1.0)
				(mizuiro::color::init::blue %= 1.0))
			/*sge::image::colors::white()*/)),
	player_times_(
		stdlib::map<player_time_map>(
			get_option<player_sequence>(
				context<machine>().cli_variables(),
				"player"),
			[](player const &p)
			{
				return 
					player_time_map::value_type(
						p,
						player_time_map::mapped_type());
			}))
{
	if (player_times_.empty())
		throw exception(FCPPT_TEXT("You have to specify at least one player (two would be even better!)"));

	fcppt::io::cout << "Ok, we've got " << player_times_.size() << " players\n";
}

insula::height_map::object &
insula::states::game_outer::height_map()
{
	return *height_map_;
}

insula::height_map::object const &
insula::states::game_outer::height_map() const
{
	return *height_map_;
}

void
insula::states::game_outer::react(
	events::tick const &)
{
	water_->update_reflection(
		[&skydome_,&height_map_,&water_]()
		{
			skydome_->render();
			height_map_->render(
				sge::renderer::state::cull_mode::off,
				water_->water_level());
		});
}

void
insula::states::game_outer::react(
	events::render const &)
{
	skydome_->render();
	height_map_->render(
		sge::renderer::state::cull_mode::back);
	water_->render();
}

insula::nugget_sequence const &
insula::states::game_outer::nugget_positions() const
{
	return nugget_positions_;
}

insula::graphics::vec2 const &
insula::states::game_outer::vehicle_position() const
{
	return vehicle_position_;
}

sge::font::metrics_ptr const 
insula::states::game_outer::large_font() const
{
	return large_font_;
}

sge::font::drawer_ptr const
insula::states::game_outer::font_drawer() const
{
	return font_drawer_;
}

insula::player const 
insula::states::game_outer::next_player() const
{
	return
		std::find_if(
			player_times_.begin(),
			player_times_.end(),
			[](player_time_map::value_type const &v) -> bool
			{
				return !static_cast<bool>(v.second);
			})->first;
}

bool
insula::states::game_outer::players_left() const
{
	return
		std::any_of(
			player_times_.begin(),
			player_times_.end(),
			[](player_time_map::value_type const &v) -> bool
			{
				return !v.second;
			});
}

void
insula::states::game_outer::place_time(
	player const &p,
	std::chrono::milliseconds const &m)
{
	// Time wasn't placed yet
	FCPPT_ASSERT(!player_times_.find(p)->second);

	player_times_.find(p)->second = 
		m;
}

insula::states::game_outer::player_time_map const &
insula::states::game_outer::player_times() const
{
	return player_times_;
}

insula::states::game_outer::~game_outer()
{
}

boost::program_options::options_description const
insula::states::game_outer::cli_options()
{
	boost::program_options::options_description opts("Outer game options");
	opts.add_options()
		("player",boost::program_options::value<player_sequence>()->multitoken(),"The players (multiple occurences needed)");
	return opts;
}
