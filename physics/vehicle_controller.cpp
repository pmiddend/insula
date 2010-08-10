#include "vehicle_controller.hpp"
#include "vehicle.hpp"
#include "scalar.hpp"
#include <sge/input/system.hpp>
#include <sge/input/key_pair.hpp>
#include <functional>

insula::physics::vehicle_controller::vehicle_controller(
	sge::input::system_ptr is,
	vehicle &_vehicle)
:
	vehicle_(
		_vehicle),
	is_active_(
		true),
	input_connection_(
		is->register_callback(
			std::bind(
				&vehicle_controller::callback,
				this,
				std::placeholders::_1)))
{
}

void
insula::physics::vehicle_controller::is_active(
	bool const _is_active)
{
	is_active_ = _is_active;
}

bool 
insula::physics::vehicle_controller::is_active() const
{
	return is_active_;
}

void
insula::physics::vehicle_controller::callback(
	sge::input::key_pair const &k)
{
	if (!is_active_)
		return;

	if (k.key().char_code() == FCPPT_TEXT('w'))
		vehicle_.engine_force(
			!k.value() ? static_cast<scalar>(0) : static_cast<scalar>(1));

	if (k.key().char_code() == FCPPT_TEXT('s'))
		vehicle_.engine_force(
			!k.value() ? static_cast<scalar>(0) : static_cast<scalar>(-1));

	if (k.key().char_code() == FCPPT_TEXT('a'))
		vehicle_.steering(
			!k.value() ? static_cast<scalar>(0) : static_cast<scalar>(1));

	if (k.key().char_code() == FCPPT_TEXT('d'))
		vehicle_.steering(
			!k.value() ? static_cast<scalar>(0) : static_cast<scalar>(-1));

	if (k.key().code() == sge::input::kc::key_space)
		vehicle_.breaking_force(
			!k.value() ? static_cast<scalar>(0) : static_cast<scalar>(1));
}
