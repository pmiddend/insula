#include "freelook.hpp"
#include "camera_move.hpp"
#include "../create_path.hpp"
#include "../music_controller.hpp"
#include "../gizmo/orthogonalize_keep_axis.hpp"
#include "../gizmo/lock_to.hpp"
#include "../model/object.hpp"
#include "../height_map/object.hpp"
#include "../graphics/camera/object.hpp"
#include "../scene/manager.hpp"
#include <sge/font/draw_text.hpp>
#include <sge/font/text_part.hpp>
#include <sge/font/align_h.hpp>
#include <sge/font/align_v.hpp>
#include <sge/font/flags_none.hpp>
#include <sge/renderer/device.hpp>
#include <sge/font/pos.hpp>
#include <sge/model/loader.hpp>
#include <sge/systems/instance.hpp>
#include <sge/image/create_texture.hpp>
#include <sge/renderer/filter/trilinear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/math/pi.hpp>
#include <fcppt/text.hpp>
#include <cmath>

insula::states::freelook::freelook(
	my_context ctx)
:
	my_base(
		ctx),
	player_backend_(
		context<game_outer>().scene_manager(),
		false,
		context<machine>().systems().renderer(),
		context<machine>().camera(),
		context<game_outer>().model_shader(),
		context<game_outer>().model_shadow_shader(),
		{
			{
				"texture",
				sge::image::create_texture(
					create_path(
						FCPPT_TEXT("player.png"),
						FCPPT_TEXT("textures")),
					context<machine>().systems().renderer(),
					context<machine>().systems().image_loader(),
					sge::renderer::filter::trilinear,
					sge::renderer::resource_flags::none)
			}
		},
		std::make_shared<model::object>(
			context<machine>().systems().md3_loader()->load(
				create_path(
					FCPPT_TEXT("player.md3"),
					FCPPT_TEXT("models")),
				sge::model::load_flags::switch_yz),
			context<machine>().systems().renderer())),
	player_instance_(
		fcppt::math::matrix::translation(
			context<game_inner>().player().gizmo().position()))
{
	context<game_outer>().scene_manager().insert(
		player_backend_,
		player_instance_);

	context<machine>().music().play_event(
		FCPPT_TEXT("freelook"));

	graphics::box const hm_size = 
		context<game_outer>().height_map().extents();

	context<machine>().camera().gizmo() = 
		context<game_outer>().shadow_object().gizmo();
}

boost::statechart::result
insula::states::freelook::react(
	events::tick const &t)
{
	context<game_outer>().react(
		t); 
	context<game_inner>().react(
		t);

	/*
	context<machine>().camera().gizmo() = 
		context<game_outer>().shadow_object().gizmo();
	*/

	return discard_event();
}

boost::statechart::result
insula::states::freelook::react(
	events::render const &r)
{
	context<game_outer>().react(
		r);

	context<game_inner>().react(
		r);

	/*
	sge::font::draw_text(
		context<game_outer>().large_font(),
		context<game_outer>().font_drawer(),
		FCPPT_TEXT("Freelook Mode\nPress return to continue"),
		sge::font::pos::null(),
		fcppt::math::dim::structure_cast<sge::font::dim>(
			context<machine>().systems().renderer()->screen_size()),
		sge::font::align_h::center,
		sge::font::align_v::center,
		sge::font::flags::none);
	*/

	return discard_event();
}

boost::statechart::result
insula::states::freelook::react(
	events::key const &r)
{
	if (r.event().key().code() == sge::input::keyboard::key_code::return_)
		return transit<camera_move>();

	return discard_event();
}

insula::states::freelook::~freelook()
{
	context<machine>().music().stop();
}
