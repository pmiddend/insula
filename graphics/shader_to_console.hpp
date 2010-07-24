#ifndef INSULA_GRAPHICS_SHADER_TO_CONSOLE_HPP_INCLUDED
#define INSULA_GRAPHICS_SHADER_TO_CONSOLE_HPP_INCLUDED

#include "shader_fwd.hpp"
#include <sge/console/arg_list.hpp>
#include <sge/console/object.hpp>
#include <fcppt/string.hpp>
#include <fcppt/signal/scoped_connection.hpp>

namespace insula
{
namespace graphics
{
class shader_to_console
{
public:
	explicit
	shader_to_console(
		fcppt::string const &name,
		shader &,
		sge::console::object &);
private:
	shader &shader_;
	fcppt::signal::scoped_connection list_con_,set_con_;

	void
	list(
		sge::console::arg_list const &,
		sge::console::object &);

	void
	set(
		sge::console::arg_list const &,
		sge::console::object &);
};
}
}

#endif
