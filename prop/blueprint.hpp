#ifndef INSULA_PROP_BLUEPRINT_HPP_INCLUDED
#define INSULA_PROP_BLUEPRINT_HPP_INCLUDED

#include "../model/backend_fwd.hpp"
#include "../physics/shared_shape_ptr.hpp"
#include "../physics/mat4.hpp"
#include "../physics/vec3.hpp"

namespace insula
{
namespace prop
{
class blueprint
{
public:
	model::backend &backend;
	physics::shared_shape_ptr shape;
	physics::vec3 rotation_axis;
	physics::scalar rotation_angle;
	physics::scalar scaling;
	physics::vec3 origin;
	physics::vec3 offset;

	explicit
	blueprint(
		model::backend &backend,
		physics::shared_shape_ptr shape,
		physics::vec3 rotation_axis,
		physics::scalar rotation_angle,
		physics::scalar scaling,
		physics::vec3 origin,
		physics::vec3 offset)
	:
		backend(backend),
		shape(shape),
		rotation_axis(rotation_axis),
		rotation_angle(rotation_angle),
		scaling(scaling),
		origin(origin),
		offset(offset)
	{
	}
};
}
}

#endif
