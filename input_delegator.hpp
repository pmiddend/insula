#ifndef INSULA_INPUT_DELEGATOR_HPP_INCLUDED
#define INSULA_INPUT_DELEGATOR_HPP_INCLUDED

#include "console/object_fwd.hpp"
#include <sge/input/system_ptr.hpp>
#include <sge/input/callback.hpp>
#include <sge/input/repeat_callback.hpp>
#include <sge/input/key_pair_function.hpp>
#include <sge/input/key_type_function.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/signal/object.hpp>

#include <fcppt/io/cout.hpp>

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
		sge::input::system_ptr,
		console::object &);

	fcppt::signal::auto_connection
	register_callback(
		sge::input::callback const &);

	fcppt::signal::auto_connection
	register_repeat_callback(
		sge::input::repeat_callback const &);

	~input_delegator() { fcppt::io::cout << "removing input_delegator\n"; }
private:
	console::object &con_;
	fcppt::signal::object<sge::input::key_pair_function> signal_;
	fcppt::signal::object<sge::input::key_type_function> repeat_signal_;

	fcppt::signal::scoped_connection c1,c2;

	void
	callback(
		sge::input::key_pair const &);

	void
	repeat_callback(
		sge::input::key_type const &);
};
}

#endif
