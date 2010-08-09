#ifndef INSULA_PHYSICS_JSON_PARSE_MODEL_HPP_INCLUDED
#define INSULA_PHYSICS_JSON_PARSE_MODEL_HPP_INCLUDED

#include "../../model/object_ptr.hpp"
#include "../../graphics/camera_fwd.hpp"
#include "../../graphics/shader_fwd.hpp"
#include "../scalar.hpp"
#include <sge/parse/json/object.hpp>
#include <sge/model/loader_ptr.hpp>
#include <sge/image/multi_loader_fwd.hpp>
#include <sge/renderer/device_ptr.hpp>

namespace insula
{
namespace physics
{
namespace json
{
insula::model::object_ptr const
parse_model(
	sge::parse::json::object const &o,
	sge::model::loader_ptr const loader,
	sge::image::multi_loader &il,
	sge::renderer::device_ptr const rend,
	insula::graphics::camera &cam,
	insula::graphics::shader &shader,
	insula::physics::scalar const scaling);
}
}
}

#endif
