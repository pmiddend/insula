#ifndef INSULA_VEHICLE_CLI_FACTORY_HPP_INCLUDED
#define INSULA_VEHICLE_CLI_FACTORY_HPP_INCLUDED

#include "object_ptr.hpp"
#include "../physics/world_fwd.hpp"
#include "../physics/vec3.hpp"
#include "../graphics/shader_fwd.hpp"
#include "../graphics/camera/object_fwd.hpp"
#include "../input_delegator.hpp"
#include "../console/object_fwd.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/image/multi_loader_fwd.hpp>
#include <sge/model/loader_ptr.hpp>
#include <sge/audio/multi_loader_fwd.hpp>
#include <sge/audio/player_ptr.hpp>
#include <boost/program_options/variables_map.hpp>

namespace insula
{
namespace vehicle
{
object_ptr const
cli_factory(
	boost::program_options::variables_map const &,
	physics::world &w,
	physics::vec3 const &position,
	sge::renderer::device_ptr const rend,
	sge::image::multi_loader &il,
	sge::model::loader_ptr const model_loader,
	graphics::shader_old &shader,
	graphics::camera::object &cam,
	input_delegator &,
	console::object &,
	sge::audio::multi_loader &,
	sge::audio::player_ptr);
}
}

#endif
