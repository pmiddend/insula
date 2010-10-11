#include "object.hpp"
#include "parameters.hpp"
#include "../input_delegator.hpp"
#include "../timed_output.hpp"
#include "../height_map/random_point.hpp"
#include "../graphics/camera/object.hpp"
#include "../json/parse_vector.hpp"
#include "../physics/vec3.hpp"
#include "../projectile/manager.hpp"
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/parse/json/float_type.hpp>
#include <sge/input/key_pair.hpp>
#include <sge/input/key_code.hpp>
#include <fcppt/text.hpp>
#include <functional>

insula::player::object::object(
	parameters const &params)
:
	camera_(
		params.camera),
	character_controller_(
		params.physics_world,
		static_cast<physics::scalar>(
			sge::parse::json::find_member_exn<sge::parse::json::float_type>(
				params.config_file.members,
				FCPPT_TEXT("width"))),
		static_cast<physics::scalar>(
			sge::parse::json::find_member_exn<sge::parse::json::float_type>(
				params.config_file.members,
				FCPPT_TEXT("height"))),
		height_map::random_point(
			params.height_map,
			params.water_level,
			params.rng_engine,
			json::parse_vector<height_map::scalar,2,sge::parse::json::float_type>(
				sge::parse::json::find_member_exn<sge::parse::json::array>(
					params.config_file.members,
					FCPPT_TEXT("flatness-range"))))),
	walk_speed_(
		static_cast<physics::scalar>(
			sge::parse::json::find_member_exn<sge::parse::json::float_type>(
				params.config_file.members,
				FCPPT_TEXT("walk-speed")))),
	input_connection_(
		params.input_delegator.register_callback(
			std::bind(
				&object::input_callback,
				this,
				std::placeholders::_1))),
	projectiles_(
		params.projectiles)
{
}

insula::physics::gizmo const
insula::player::object::gizmo() const
{
	return 
		character_controller_.gizmo();
}

void
insula::player::object::update(
	time_delta const delta)
{
	physics::vec3 walk = physics::vec3::null();
	if (action_execute_[action::forward])
		walk += physics::vec3(-camera_.gizmo().forward().x(),0,-camera_.gizmo().forward().z());
	if (action_execute_[action::backward])
		walk += physics::vec3(camera_.gizmo().forward().x(),0,camera_.gizmo().forward().z());
		//walk += camera_.gizmo().forward();
	if (action_execute_[action::left])
		walk += -camera_.gizmo().right();
	if (action_execute_[action::right])
		walk += camera_.gizmo().right();
	if (walk != physics::vec3::null())
		walk = fcppt::math::vector::normalize(walk) * walk_speed_;
	
	character_controller_.walk_vector(
		walk);
	character_controller_.update(
		delta);
	camera_.gizmo().position(
		character_controller_.gizmo().position());
}

void
insula::player::object::input_callback(
	sge::input::key_pair const &k)
{
	switch (k.key().code())
	{
		case sge::input::kc::key_w:
			action_execute_[action::forward] = k.value();
			break;
		case sge::input::kc::key_s:
			action_execute_[action::backward] = k.value();
			break;
		case sge::input::kc::key_a:
			action_execute_[action::left] = k.value();
			break;
		case sge::input::kc::key_d:
			action_execute_[action::right] = k.value();
			break;
		case sge::input::kc::mouse_l:
			if (k.value())
			{
				projectiles_.spawn(
					camera_.gizmo().position() - 
						4.0f * camera_.gizmo().forward(),
					-camera_.gizmo().forward() * 20.0f);
			}
			break;
		default:
			break;
	}
}
