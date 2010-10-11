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
#include <sge/input/key_pair.hpp>
#include <sge/input/key_code.hpp>
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
		/*
		insula::gizmo::lock_to(
			insula::gizmo::orthogonalize_keep_axis(
				graphics::gizmo(
					graphics::gizmo::init()
						.position(
							graphics::vec3(
								hm_size.w() / 
								static_cast<graphics::scalar>(2),
								static_cast<graphics::scalar>(0),
								hm_size.d() / 
								static_cast<graphics::scalar>(2)))
						.forward(
							graphics::vec3(
								std::cos(
									fcppt::math::pi<graphics::scalar>()*
									static_cast<graphics::scalar>(1.0/4.0)),
								static_cast<graphics::scalar>(0),
								std::sin(
									fcppt::math::pi<graphics::scalar>()*
									static_cast<graphics::scalar>(1.0/4.0))))
						.right(
							graphics::vec3(
								std::cos(
									fcppt::math::pi<graphics::scalar>()*
									static_cast<graphics::scalar>(-1.0/4.0)),
								static_cast<graphics::scalar>(0),
								std::sin(
									fcppt::math::pi<graphics::scalar>()*
									static_cast<graphics::scalar>(-1.0/4.0))))
						.up(
							graphics::vec3(
								static_cast<graphics::scalar>(0),
								static_cast<graphics::scalar>(1),
								static_cast<graphics::scalar>(0)))),
					0),
				hm_size.w()/
				static_cast<graphics::scalar>(
					2),
				fcppt::math::pi<graphics::scalar>()/
				static_cast<graphics::scalar>(4))*/;
}

boost::statechart::result
insula::states::freelook::react(
	events::tick const &t)
{
	context<game_outer>().react(
		t); 
	context<game_inner>().react(
		t);

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
	if (r.pair().key().code() == sge::input::kc::key_return)
		return transit<camera_move>();

	return discard_event();
}

insula::states::freelook::~freelook()
{
	context<machine>().music().stop();
}
