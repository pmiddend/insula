#include "input.hpp"
#include "../input_delegator.hpp"
#include "../physics/vehicle/object.hpp"
#include "../physics/scalar.hpp"
#include <sge/input/key_pair.hpp>
#include <fcppt/io/cout.hpp>
#include <functional>

insula::vehicle::input::input(
	input_delegator &is,
	physics::vehicle::object &_vehicle)
:
	vehicle_(
		_vehicle),
	is_active_(
		true),
	input_connection_(
		is.register_callback(
			std::bind(
				&input::callback,
				this,
				std::placeholders::_1))),
	key_pressed_(
		{
			{'w',false},
			{'s',false},
			{'a',false},
			{'d',false}
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
insula::vehicle::input::callback(
	sge::input::key_pair const &k)
{
	if (!is_active_)
		return;

	if (key_pressed_.find(k.key().char_code()) != key_pressed_.end())
	{
		// Does char/int really convert properly to true/false?
		key_pressed_[k.key().char_code()] = k.value() ? true : false;
	}
	else
	{
		if (k.key().code() == sge::input::kc::key_space)
			vehicle_.breaking_force(
				!k.value() 
				? 
					static_cast<physics::scalar>(0) 
				: 
					static_cast<physics::scalar>(1));
	}

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
}
