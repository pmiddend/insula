#ifndef INSULA_HEIGHT_MAP_CLI_FACTORY_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_CLI_FACTORY_HPP_INCLUDED

#include "object_ptr.hpp"
#include "../graphics/camera_fwd.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/console/object_fwd.hpp>
#include <sge/image/multi_loader_fwd.hpp>
#include <boost/program_options/variables_map.hpp>

namespace insula
{
namespace height_map
{
object_ptr const
cli_factory(
	boost::program_options::variables_map const &,
	graphics::camera &,
	sge::renderer::device_ptr,
	sge::image::multi_loader &);
}
}

#endif
