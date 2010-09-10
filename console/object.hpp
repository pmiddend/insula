#ifndef INSULA_CONSOLE_OBJECT_HPP_INCLUDED
#define INSULA_CONSOLE_OBJECT_HPP_INCLUDED

#include "redirect_mode.hpp"
#include "streambuf.hpp"
#include <sge/input/system_ptr.hpp>
#include <sge/console/object.hpp>
#include <sge/console/gfx.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/font/system_ptr.hpp>
#include <sge/image/multi_loader_fwd.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/signal/object.hpp>
#include <memory>

namespace insula
{
namespace console
{
/// This is just a container for the console "model" (so
/// sge::console::object) and the "view" (sge::console::gfx). Because
/// that's a little boring, it also provides cout/cerr/clog
/// redirection to the console
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
		sge::image::multi_loader &,
		redirect_mode::type);
	
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

	~object();
private:
	typedef
	std::unique_ptr<streambuf>
	unique_scoped_streambuf;

	sge::console::object object_;
	sge::console::gfx gfx_;
	unique_scoped_streambuf cout_,clog_,cerr_;
};
}
}

#endif
