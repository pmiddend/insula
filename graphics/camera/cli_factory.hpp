#ifndef INSULA_GRAPHICS_CAMERA_CLI_FACTORY_HPP_INCLUDED
#define INSULA_GRAPHICS_CAMERA_CLI_FACTORY_HPP_INCLUDED

#include "object_ptr.hpp"
#include "../../input_delegator.hpp"
#include "../vec3.hpp"
#include "../scalar.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/image/multi_loader_fwd.hpp>
#include <boost/program_options/variables_map.hpp>

namespace insula
{
namespace graphics
{
namespace camera
{
object_ptr const
cli_factory(
	boost::program_options::variables_map const &,
	input_delegator &,
	scalar aspect,
	vec3 const &position);
}
}
}

#endif