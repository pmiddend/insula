#include "mirror_camera.hpp"
#include "../graphics/gizmo_init.hpp"
#include "../graphics/scalar.hpp"
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/cross.hpp>

std::tuple<insula::graphics::vec3,insula::graphics::gizmo> const
insula::water::mirror_camera(
	graphics::vec3 const &old_position,
	graphics::gizmo const &axes,
	graphics::scalar const water_height)
{
	graphics::vec3 const 
		new_position(
			old_position.x(),
			-old_position.y() + static_cast<graphics::scalar>(2)*water_height,
			old_position.z()),
		forward_target(
			old_position + axes.forward()),
		new_forward_target(
			graphics::vec3(
				forward_target.x(),
				-forward_target.y() + static_cast<graphics::scalar>(2)*water_height,
				forward_target.z())),
		new_forward_(
			new_forward_target - new_position),
		up_target(
			old_position + axes.up()),
		new_up_target(
			graphics::vec3(
				up_target.x(),
				-up_target.y() + static_cast<graphics::scalar>(2)*water_height,
				up_target.z())),
		new_up_(
			new_up_target - new_position),
		new_right_(
			cross(
				new_up_,
				new_forward_)),
		new_up(
			cross(
				new_forward_,
				new_right_)),
		new_forward(
			cross(new_right_,
			new_up)),
		new_right(
			cross(new_up,
			new_forward));

	return 
		std::tuple<insula::graphics::vec3,insula::graphics::gizmo>(
			new_position,
			graphics::gizmo(
				graphics::gizmo_init()
					.forward(new_forward)
					.right(new_right)
					.up(new_up)));
}
