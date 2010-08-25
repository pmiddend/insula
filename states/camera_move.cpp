#include "camera_move.hpp"
#include "../sound_controller.hpp"
#include "../events/tick.hpp"
#include "../graphics/scalar.hpp"
#include "../graphics/camera/object.hpp"
#include "../vehicle/object.hpp"
#include "../events/render.hpp"
#include <fcppt/math/vector/arithmetic.hpp>

insula::states::camera_move::camera_move(
	my_context ctx)
:
	my_base(
		ctx),
	target_gizmo_(
		context<game_inner>().vehicle().lock_to_gizmo())
{
	context<machine>().sounds().play(
		FCPPT_TEXT("camera_swoosh"));
}

boost::statechart::result
insula::states::camera_move::react(
	events::tick const &t)
{
	context<game_outer>().react(
		t); 
	context<game_inner>().react(
		t);

	graphics::scalar const cam_speed = 
		static_cast<graphics::scalar>(0.25);

	context<machine>().camera().gizmo().position(
		context<machine>().camera().gizmo().position() + 
		static_cast<graphics::scalar>(t.delta()) * 
		(target_gizmo_.position() - context<machine>().camera().gizmo().position())/
		cam_speed);

	return discard_event();
}

boost::statechart::result
insula::states::camera_move::react(
	events::render const &r)
{
	context<game_outer>().react(
		r);

	context<game_inner>().react(
		r);

	return discard_event();
}
