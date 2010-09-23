#include "input.hpp"
#include "parameters.hpp"
#include "../input_delegator.hpp"
#include "../physics/vehicle/object.hpp"
#include "../physics/scalar.hpp"
#include <sge/input/key_pair.hpp>
#include <fcppt/math/signum.hpp>
#include <functional>
#include <iostream>

insula::vehicle::input::input(
	physics::vehicle::object &_vehicle,
	parameters const &params)
:
	vehicle_(
		_vehicle),
	is_active_(
		true),
	input_connection_(
		params.input_delegator_.register_callback(
			std::bind(
				&input::callback,
				this,
				std::placeholders::_1))),
	steering_speed_(
		params.steering_speed),
	braking_speed_(
		params.braking_speed),
	engine_speed_(
		params.engine_speed),
	steering_(
		static_cast<physics::scalar>(0)),
	brake_(
		static_cast<physics::scalar>(0)),
	engine_(
		static_cast<physics::scalar>(0)),
	key_pressed_(
		{
			{action_accelerate,false},
			{action_reverse,false},
			{action_brake,false},
			{action_left,false},
			{action_right,false}
		})
{
}

void
insula::vehicle::input::is_active(
	bool const _is_active)
{
	is_active_ = _is_active;
}

bool 
insula::vehicle::input::is_active() const
{
	return is_active_;
}

void
insula::vehicle::input::update(
	time_delta const delta)
{
	// The engine should start slowly but stop quickly
	if (key_pressed_[action_accelerate])
		engine_ =
			std::min(
				static_cast<physics::scalar>(1),
				engine_ + engine_speed_ * delta);

	// The engine should start slowly but stop quickly
	if (key_pressed_[action_reverse])
		engine_ =
			std::max(
				static_cast<physics::scalar>(-1),
				engine_ - engine_speed_ * delta);

	if (!key_pressed_[action_accelerate] && !key_pressed_[action_reverse])
		engine_ = 0;

	// Braking should start slowly but stop quickly
	if (key_pressed_[action_brake])
		brake_ =
			std::min(
				static_cast<physics::scalar>(1),
				brake_ + braking_speed_ * delta);
	else
		brake_ = 
			static_cast<physics::scalar>(0);

	if (key_pressed_[action_left])
		steering_ = 
			std::min(
				static_cast<physics::scalar>(1),
				steering_ + steering_speed_ * delta);
	
	if (key_pressed_[action_right])
		steering_ = 
			std::max(
				static_cast<physics::scalar>(-1),
				steering_ - steering_speed_ * delta);
	
	if (!key_pressed_[action_left] && !key_pressed_[action_right])
	{
		if (std::abs(steering_) < 0.01)
			steering_ = 0;
		else
			steering_ += 
				static_cast<physics::scalar>(-1) * 
				fcppt::math::signum(steering_) * 
				steering_speed_ * 
				delta;
	}

	vehicle_.engine_force(engine_);
	vehicle_.breaking_force(brake_);
	vehicle_.steering(steering_);
}

void
insula::vehicle::input::callback(
	sge::input::key_pair const &k)
{
	if (!is_active_)
		return;

	if (k.key().char_code() == FCPPT_TEXT('w'))
		key_pressed_[action_accelerate] = k.value();
	if (k.key().char_code() == FCPPT_TEXT('s'))
		key_pressed_[action_reverse] = k.value();
	else if (k.key().char_code() == FCPPT_TEXT('a'))
		key_pressed_[action_left] = k.value();
	else if (k.key().char_code() == FCPPT_TEXT('d'))
		key_pressed_[action_right] = k.value();
	else if (k.key().code() == sge::input::kc::key_space)
		key_pressed_[action_brake] = k.value();

#if 0
	bool const 
		drive_forward = 
			key_pressed_[FCPPT_TEXT('w')],
		drive_sideways = 
			key_pressed_[FCPPT_TEXT('a')] || 
			key_pressed_[FCPPT_TEXT('d')];

	physics::scalar forward_force = 0;

	if (key_pressed_[FCPPT_TEXT('w')])
		forward_force = 1;
	if (key_pressed_[FCPPT_TEXT('s')])
		forward_force += -1;

	physics::scalar sideways_force = 0;

	if (key_pressed_[FCPPT_TEXT('a')])
		sideways_force = 1;
	if (key_pressed_[FCPPT_TEXT('d')])
		sideways_force += -1;

//	if (drive_sideways && vehicle_.speed_kmh() > 30)
//		forward_force = 0;

	vehicle_.engine_force(forward_force);
	vehicle_.steering(sideways_force);
#endif
}
