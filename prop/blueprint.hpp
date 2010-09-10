#ifndef INSULA_PROP_BLUEPRINT_HPP_INCLUDED
#define INSULA_PROP_BLUEPRINT_HPP_INCLUDED

#include "../model_backend_fwd.hpp"
#include "../physics/shared_shape_ptr.hpp"
#include "../physics/mat4.hpp"
#include "../physics/vec3.hpp"
#include "../physics/solidity.hpp"
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>

namespace insula
{
namespace prop
{
class blueprint
{
public:
	model_backend &backend;
	physics::shared_shape_ptr shape;
	physics::vec3 rotation_axis;
	physics::scalar rotation_angle;
	physics::scalar scaling;
	physics::vec3 origin;
	physics::vec3 offset;
	physics::solidity::type solidity;

	explicit
	blueprint(
		model_backend &backend,
		physics::shared_shape_ptr shape,
		physics::vec3 rotation_axis,
		physics::scalar rotation_angle,
		physics::scalar scaling,
		physics::vec3 origin,
		physics::vec3 offset,
		physics::solidity::type solidity)
	:
		backend(backend),
		shape(shape),
		rotation_axis(rotation_axis),
		rotation_angle(rotation_angle),
		scaling(scaling),
		origin(origin),
		offset(offset),
		solidity(solidity)
	{
	}
};
}
}

#endif