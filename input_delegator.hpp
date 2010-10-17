#ifndef INSULA_INPUT_DELEGATOR_HPP_INCLUDED
#define INSULA_INPUT_DELEGATOR_HPP_INCLUDED

#include "console/object_fwd.hpp"
#include <sge/systems/instance_fwd.hpp>
#include <sge/input/keyboard/key_function.hpp>
#include <sge/input/mouse/axis_function.hpp>
#include <sge/input/mouse/button_function.hpp>
#include <sge/input/keyboard/key_callback.hpp>
#include <sge/input/keyboard/key_event_fwd.hpp>
#include <sge/input/mouse/axis_callback.hpp>
#include <sge/input/mouse/button_callback.hpp>
#include <sge/input/mouse/button_event_fwd.hpp>
#include <sge/input/mouse/axis_event_fwd.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/signal/object.hpp>

namespace insula
{
/// The input delegator is a layer between the input system and its
/// users. One use case for this is the console: The console input -
/// if the console is active - should always override any other input,
/// so that typing text doesn't also drive the vehicle. Thats why the
/// input delegator gets the console in the constructor.
class input_delegator
{
public:
	input_delegator(input_delegator const &) = delete;
	input_delegator &operator=(input_delegator const &) = delete;

	explicit 
	input_delegator(
		sge::systems::instance const &,
		console::object &);

	fcppt::signal::auto_connection
	key_callback(
		sge::input::keyboard::key_callback const &);

	fcppt::signal::auto_connection
	mouse_axis_callback(
		sge::input::mouse::axis_callback const &);

	fcppt::signal::auto_connection
	mouse_button_callback(
		sge::input::mouse::button_callback const &);
private:
	console::object &con_;
	fcppt::signal::object<sge::input::keyboard::key_function> key_signal_;
	fcppt::signal::object<sge::input::mouse::axis_function> mouse_axis_signal_;
	fcppt::signal::object<sge::input::mouse::button_function> mouse_button_signal_;
	fcppt::signal::scoped_connection c1,c2,c3;

	void
	key_callback_internal(
		sge::input::keyboard::key_event const &);

	void
	mouse_axis_callback_internal(
		sge::input::mouse::axis_event const &);

	void
	mouse_button_callback_internal(
		sge::input::mouse::button_event const &);
};
}

#endif
