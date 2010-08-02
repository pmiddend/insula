#ifndef INSULA_HEIGHT_MAP_CONSOLE_PROXY_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_CONSOLE_PROXY_HPP_INCLUDED

#include "object_fwd.hpp"
#include "../graphics/shader_to_console.hpp"
#include <sge/console/object_fwd.hpp>
#include <sge/console/arg_list.hpp>
#include <sge/image/multi_loader_fwd.hpp>
#include <fcppt/signal/scoped_connection.hpp>

namespace insula
{
namespace height_map
{
class console_proxy
{
public:
	console_proxy(console_proxy const &) = delete;
	console_proxy &operator=(console_proxy const &) = delete;
	
	explicit
	console_proxy(
		object &,
		sge::console::object &,
		sge::image::multi_loader &);
private:
	graphics::shader_to_console shader_;
	fcppt::signal::scoped_connection regenerate_connection_;
	sge::image::multi_loader &image_loader_;
	object &obj_;

	void
	regenerate(
		sge::console::arg_list const &,
		sge::console::object &);
};
}
}

#endif
