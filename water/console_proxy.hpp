#ifndef INSULA_WATER_CONSOLE_PROXY_HPP_INCLUDED
#define INSULA_WATER_CONSOLE_PROXY_HPP_INCLUDED

#include "object_fwd.hpp"
#include "../graphics/shader_to_console.hpp"
#include <sge/console/object_fwd.hpp>
#include <sge/console/arg_list.hpp>
#include <fcppt/signal/scoped_connection.hpp>

namespace insula
{
namespace water
{
class console_proxy
{
public:
	console_proxy(console_proxy const &) = delete;
	console_proxy &operator=(console_proxy const &) = delete;
	
	explicit
	console_proxy(
		object &,
		sge::console::object &);
private:
	graphics::shader_to_console shader_;
};
}
}

#endif
