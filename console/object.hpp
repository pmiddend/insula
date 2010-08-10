#ifndef INSULA_CONSOLE_OBJECT_HPP_INCLUDED
#define INSULA_CONSOLE_OBJECT_HPP_INCLUDED

#include <sge/input/system_ptr.hpp>
#include <sge/console/object.hpp>
#include <sge/console/gfx.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/font/system_ptr.hpp>
#include <sge/image/multi_loader_fwd.hpp>
#include <sge/font/system_ptr.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/signal/object.hpp>

namespace insula
{
namespace console
{
class object
{
public:
	object &operator=(
		object const &) = delete;
	object(
		object const &) = delete;

	
	explicit
	object(
		sge::input::system_ptr,
		sge::renderer::device_ptr,
		sge::font::system_ptr,
		sge::image::multi_loader &);
	
	void
	render();

	sge::console::object &
	model();

	sge::console::object const &
	model() const;

	sge::console::gfx &
	view();

	sge::console::gfx const &
	view() const;
private:
	sge::console::object object_;
	sge::console::gfx gfx_;
};
}
}

#endif
