#include "input_delegator.hpp"
#include "console/object.hpp"
#include <sge/input/system.hpp>
#include <sge/input/key_code.hpp>
#include <sge/input/key_pair.hpp>
#include <fcppt/math/almost_zero.hpp>
#include <functional>

insula::input_delegator::input_delegator(
	sge::input::system_ptr const is,
	console::object &_con)
:
	con_(
		_con),
	c1(
		is->register_callback(
			std::bind(
				&input_delegator::callback,
				this,
				std::placeholders::_1))),
	c2(
		is->register_repeat_callback(
			std::bind(
				&input_delegator::repeat_callback,
				this,
				std::placeholders::_1)))
{
}

fcppt::signal::auto_connection
insula::input_delegator::register_callback(
	sge::input::callback const &c)
{
	return signal_.connect(c);
}

fcppt::signal::auto_connection
insula::input_delegator::register_repeat_callback(
	sge::input::repeat_callback const &c)
{
	return repeat_signal_.connect(c);
}

void
insula::input_delegator::callback(
	sge::input::key_pair const &c)
{
	if(
		c.key().code() == sge::input::kc::key_f1 && 
		!fcppt::math::almost_zero(c.value()))
		con_.view().active(
			!con_.view().active());
	if (!con_.view().active())
		signal_(
			c);
}

void
insula::input_delegator::repeat_callback(
	sge::input::key_type const &c)
{
	if (!con_.view().active())
		repeat_signal_(
			c);
}
