#include "input_delegator.hpp"
#include "console/object.hpp"
#include <sge/systems/instance.hpp>
#include <sge/input/keyboard/collector.hpp>
#include <sge/input/mouse/collector.hpp>
#include <sge/input/keyboard/key_event.hpp>
#include <sge/input/keyboard/key.hpp>
#include <fcppt/math/almost_zero.hpp>
#include <functional>

insula::input_delegator::input_delegator(
	sge::systems::instance const &systems,
	console::object &_con)
:
	con_(
		_con),
	c1(
		systems.keyboard_collector()->key_callback(
			std::bind(
				&input_delegator::key_callback_internal,
				this,
				std::placeholders::_1))),
	c2(
		systems.mouse_collector()->axis_callback(
			std::bind(
				&input_delegator::mouse_axis_callback_internal,
				this,
				std::placeholders::_1))),
	c3(
		systems.mouse_collector()->button_callback(
			std::bind(
				&input_delegator::mouse_button_callback_internal,
				this,
				std::placeholders::_1)))
{
}

fcppt::signal::auto_connection
insula::input_delegator::key_callback(
	sge::input::keyboard::key_callback const &c)
{
	return key_signal_.connect(c);
}

fcppt::signal::auto_connection
insula::input_delegator::mouse_axis_callback(
	sge::input::mouse::axis_callback const &c)
{
	return mouse_axis_signal_.connect(c);
}

fcppt::signal::auto_connection
insula::input_delegator::mouse_button_callback(
	sge::input::mouse::button_callback const &c)
{
	return mouse_button_signal_.connect(c);
}

void
insula::input_delegator::key_callback_internal(
	sge::input::keyboard::key_event const &c)
{
	if(
		c.key().code() == sge::input::keyboard::key_code::f1 && 
		!c.pressed())
		con_.view().active(
			!con_.view().active());
	if (!con_.view().active())
		key_signal_(
			c);
}

void
insula::input_delegator::mouse_axis_callback_internal(
	sge::input::mouse::axis_event const &e)
{
	mouse_axis_signal_(
		e);
}

void
insula::input_delegator::mouse_button_callback_internal(
	sge::input::mouse::button_event const &b)
{
	mouse_button_signal_(
		b);
}
