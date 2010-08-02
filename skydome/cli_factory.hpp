#ifndef INSULA_SKYDOME_CLI_FACTORY_HPP_INCLUDED
#define INSULA_SKYDOME_CLI_FACTORY_HPP_INCLUDED

#include "object_ptr.hpp"
#include "../graphics/camera_fwd.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/image/multi_loader_fwd.hpp>
#include <boost/program_options/variables_map.hpp>

namespace insula
{
namespace skydome
{
object_ptr const
cli_factory(
	boost::program_options::variables_map const &,
	graphics::camera &,
	sge::renderer::device_ptr);
}
}

#endif
