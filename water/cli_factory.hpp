#ifndef INSULA_WATER_CLI_FACTORY_HPP_INCLUDED
#define INSULA_WATER_CLI_FACTORY_HPP_INCLUDED

#include "object_ptr.hpp"
#include "../graphics/camera/object_fwd.hpp"
#include "../graphics/box.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/image/multi_loader_fwd.hpp>
#include <boost/program_options/variables_map.hpp>

namespace insula
{
namespace water
{
object_ptr const
cli_factory(
	boost::program_options::variables_map const &,
	sge::renderer::device_ptr,
	graphics::camera::object &,
	graphics::box const &,
	sge::image::multi_loader &);
}
}

#endif
