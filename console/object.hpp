#ifndef INSULA_CONSOLE_OBJECT_HPP_INCLUDED
#define INSULA_CONSOLE_OBJECT_HPP_INCLUDED

#include <sge/input/system_ptr.hpp>
#include <sge/input/callback.hpp>
#include <sge/input/repeat_callback.hpp>
#include <sge/input/key_pair_function.hpp>
#include <sge/input/key_type_function.hpp>
#include <sge/console/object.hpp>
#include <sge/font/system_ptr.hpp>
#include <sge/console/gfx.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/image/multi_loader_fwd.hpp>
#include <sge/font/system_ptr.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/signal/object.hpp>

namespace insula
{
namespace console
{
class object
{
FCPPT_NONCOPYABLE(object)
public:
	explicit
	object(
		sge::input::system_ptr,
		sge::renderer::device_ptr,
		sge::font::system_ptr,
		sge::image::multi_loader &);
	
	fcppt::signal::auto_connection
	register_callback(
		sge::input::callback const &);

	fcppt::signal::auto_connection
	register_repeat_callback(
		sge::input::repeat_callback const &);
	
	void
	render();
private:
	fcppt::signal::object<sge::input::key_pair_function> signal_;
	fcppt::signal::object<sge::input::key_type_function> repeat_signal_;

	sge::console::object object_;
	sge::console::gfx gfx_;

	fcppt::signal::scoped_connection c1,c2;

	void
	callback(
		sge::input::key_pair const &);

	void
	repeat_callback(
		sge::input::key_type const &);
};
}
}

#endif
