#ifndef INSULA_PHYSICS_JSON_PARSE_VEHICLE_HPP_INCLUDED
#define INSULA_PHYSICS_JSON_PARSE_VEHICLE_HPP_INCLUDED

#include "../vehicle/object_ptr.hpp"
#include "../vec3.hpp"
#include "../world_fwd.hpp"
#include "../../graphics/shader_fwd.hpp"
#include "../../graphics/camera/object_fwd.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/image/multi_loader_fwd.hpp>
#include <sge/model/loader_ptr.hpp>
#include <sge/parse/json/object.hpp>
#include <fcppt/filesystem/path.hpp>

namespace insula
{
namespace physics
{
namespace json
{
vehicle::object_ptr const
parse_vehicle(
	sge::parse::json::object const &,
	world &w,
	vec3 const &position,
	sge::renderer::device_ptr const rend,
	sge::image::multi_loader &il,
	sge::model::loader_ptr const model_loader,
	graphics::shader_old &shader,
	graphics::camera::object &cam);
}
}
}

#endif
