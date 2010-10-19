#include "game_outer.hpp"
#include "../random_seed.hpp"
#include "../model/create_shader.hpp"
#include "../model/create_shadow_shader.hpp"
#include "../ghost/manager_parameters.hpp"
#include "../stdlib/map.hpp"
#include "../overlay/parameters.hpp"
#include "../water/parameters.hpp"

#include "../height_map/object.hpp"
#include "../height_map/parameters.hpp"
#include "../height_map/image_to_array.hpp"
#include "../skydome/object.hpp"
#include "../skydome/parameters.hpp"
#include "../nugget/parameters.hpp"
#include "../prop/parameters.hpp"
#include "../water/object.hpp"
#include "../json/parse_font.hpp"
#include "../json/find_member.hpp"
#include "../exception.hpp"
#include "../scene/render_pass/object.hpp"
#include "../shadow/parameters.hpp"
#include "../timed_output.hpp"
#include <fcppt/math/box/structure_cast.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/text.hpp>
#include <sge/font/drawer_3d.hpp>
#include <sge/parse/json/string.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/color/rgb8.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/default_target.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/renderer/pixel_pos.hpp>
#include <sge/renderer/device.hpp>
#include <mizuiro/color/init.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <algorithm>

insula::states::game_outer::game_outer(
	my_context ctx)
:
	my_base(
		ctx),
	model_shader_(
		model::create_shader(
			context<machine>().systems().renderer(),
			json::find_member<graphics::vec3>(
				context<machine>().config_file(),
				"sun-position"))),
	model_shadow_shader_(
		model::create_shadow_shader(
			context<machine>().systems().renderer())),
	scene_manager_(
		context<machine>().systems().renderer(),
		context<machine>().camera()),
	shadow_(
		shadow::parameters(
			context<machine>().systems(),
			graphics::gizmo(
				graphics::gizmo::init()
					.position(
						graphics::vec3(
							303.661f,592.32f,376.59f))
					.forward(
						graphics::vec3(
							-0.315495f,0.915277f,-0.250462f))
					.right(
						graphics::vec3(
							-0.621763f,0.0f,0.783205f))
					.up(
						graphics::vec3(
							0.71685f,0.402825f,0.569086f))),
				json::find_member<sge::renderer::dim_type>(
					context<machine>().config_file(),
					FCPPT_TEXT("shadow-map-size")),
			scene_manager_)),
	height_map_(
		insula::height_map::parameters(
			context<machine>().config_file(),
			context<machine>().camera(),
			context<machine>().systems().renderer(),
			context<machine>().systems().image_loader(),
			scene_manager_,
			shadow_.texture(),
			shadow_.mvp(
				context<machine>().camera().perspective()))),
	skydome_(
		skydome::parameters(
			context<machine>().config_file(),
			context<machine>().camera(),
			context<machine>().systems(),
			scene_manager_)),
	water_(
		water::parameters(
			context<machine>().config_file(),
			context<machine>().systems(),
			context<machine>().camera(),
			fcppt::math::box::structure_cast<graphics::box>(
				height_map_.extents()),
			scene_manager_)),
	large_font_(
		json::parse_font(
			json::find_member<sge::parse::json::object>(
				context<machine>().config_file(),
				FCPPT_TEXT("large_font")),
			context<machine>().systems().font_system())),
	font_drawer_(
		fcppt::make_shared_ptr<sge::font::drawer_3d>(
			context<machine>().systems().renderer(),
			sge::image::color::rgb8(
				(mizuiro::color::init::red %= 1.0)
				(mizuiro::color::init::green %= 1.0)
				(mizuiro::color::init::blue %= 1.0)))),
	player_times_(
		stdlib::map<player_time_map>(
			json::find_member<player_sequence>(
				context<machine>().config_file(),
				"players"),
			[](fcppt::string const &p)
			{
				return 
					player_time_map::value_type(
						p,
						player_time_map::mapped_type());
			})),
	broadphase_manager_(
		context<machine>().camera()),
	prop_manager_(
		prop::parameters(
			context<machine>().config_file(),
			context<machine>().systems(),
			context<machine>().camera(),
			*model_shader_,
			*model_shadow_shader_,
			height_map_,
			water_.water_level(),
			scene_manager_,
			broadphase_manager_)),
	ghost_manager_(
		ghost::manager_parameters(
			scene_manager_,
			broadphase_manager_,
			json::find_member<sge::parse::json::array>(
				context<machine>().config_file(),
				FCPPT_TEXT("ghosts")),
			context<machine>().systems(),
			context<machine>().camera(),
			*model_shader_,
			*model_shadow_shader_,
			height_map_,
			static_cast<height_map::scalar>(
				water_.water_level()))),
	player_position_rng_(
		random_seed()),
	overlay_(
		overlay::parameters(
			context<machine>().systems(),
			scene_manager_,
			context<machine>().camera()))
{
	if (player_times_.empty())
		throw exception(FCPPT_TEXT("You have to specify at least one player"));
}

insula::height_map::object &
insula::states::game_outer::height_map()
{
	return height_map_;
}

insula::height_map::object const &
insula::states::game_outer::height_map() const
{
	return height_map_;
}

void
insula::states::game_outer::react(
	events::tick const &)
{
	broadphase_manager_.update();
}

void
insula::states::game_outer::react(
	events::render const &)
{
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

fcppt::string const 
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
	fcppt::string const &p,
	std::chrono::milliseconds const &m)
{
	// Time wasn't placed yet
	FCPPT_ASSERT(
		!player_times_.find(p)->second);

	player_times_.find(p)->second = 
		m;
}

insula::states::game_outer::player_time_map const &
insula::states::game_outer::player_times() const
{
	return player_times_;
}

insula::graphics::shader::object &
insula::states::game_outer::model_shader()
{
	return *model_shader_;
}

insula::graphics::shader::object &
insula::states::game_outer::model_shadow_shader()
{
	return *model_shadow_shader_;
}

insula::scene::manager &
insula::states::game_outer::scene_manager()
{
	return scene_manager_;
}

insula::prop::manager &
insula::states::game_outer::prop_manager()
{
	return prop_manager_;
}

insula::physics::broadphase::manager &
insula::states::game_outer::broadphase_manager()
{
	return broadphase_manager_;
}

insula::random_engine &
insula::states::game_outer::player_position_rng()
{
	return player_position_rng_;
}

insula::graphics::scalar
insula::states::game_outer::water_level() const
{
	return water_.water_level();
}

insula::states::game_outer::~game_outer()
{
}
