#include "input.hpp"
#include "../input_delegator.hpp"
#include "../physics/vehicle/object.hpp"
#include "../physics/scalar.hpp"
#include <sge/input/key_pair.hpp>
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
				std::placeholders::_1)))
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

	if (k.key().char_code() == FCPPT_TEXT('w'))
		vehicle_.engine_force(
			!k.value() ? static_cast<physics::scalar>(0) : static_cast<physics::scalar>(1));

	if (k.key().char_code() == FCPPT_TEXT('s'))
		vehicle_.engine_force(
			!k.value() ? static_cast<physics::scalar>(0) : static_cast<physics::scalar>(-1));

	if (k.key().char_code() == FCPPT_TEXT('a'))
		vehicle_.steering(
			!k.value() ? static_cast<physics::scalar>(0) : static_cast<physics::scalar>(1));

	if (k.key().char_code() == FCPPT_TEXT('d'))
		vehicle_.steering(
			!k.value() ? static_cast<physics::scalar>(0) : static_cast<physics::scalar>(-1));

	if (k.key().code() == sge::input::kc::key_space)
		vehicle_.breaking_force(
			!k.value() ? static_cast<physics::scalar>(0) : static_cast<physics::scalar>(1));
}
