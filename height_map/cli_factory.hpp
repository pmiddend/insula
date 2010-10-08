#ifndef INSULA_HEIGHT_MAP_CLI_FACTORY_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_CLI_FACTORY_HPP_INCLUDED

#include "object_ptr.hpp"
#include "../graphics/camera/object_fwd.hpp"
#include "../graphics/mat4.hpp"
#include "../scene/manager_fwd.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <sge/image/multi_loader_fwd.hpp>
#include <boost/program_options/variables_map.hpp>

namespace insula
{
namespace height_map
{
object_ptr const
cli_factory(
	boost::program_options::variables_map const &,
	graphics::camera::object &,
	sge::renderer::device_ptr,
	sge::image::multi_loader &,
	scene::manager &,
	sge::renderer::texture_ptr shadow_tex,
	graphics::mat4 const &shadow_mvp);
}
}

#endif
