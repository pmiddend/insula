#ifndef INSULA_GRAPHICS_CAMERA_PARAMETERS_HPP_INCLUDED
#define INSULA_GRAPHICS_CAMERA_PARAMETERS_HPP_INCLUDED

#include "../../input_delegator_fwd.hpp"
#include "../scalar.hpp"
#include "../vec3.hpp"
#include <fcppt/math/vector/basic_impl.hpp>

namespace insula
{
namespace graphics
{
namespace camera
{
class parameters
{
public:	
	insula::input_delegator &input_delegator;
	scalar aspect;
	scalar fov;
	scalar near;
	scalar far;
	scalar movement_speed;
	scalar rotation_speed;
	vec3 position;

	explicit
	parameters(
		insula::input_delegator &input_delegator,
		scalar aspect,
		scalar fov,
		scalar near,
		scalar far,
		scalar movement_speed,
		scalar rotation_speed,
		vec3 const &position)
	:
		input_delegator(input_delegator),
		aspect(aspect),
		fov(fov),
		near(near),
		far(far),
		movement_speed(movement_speed),
		rotation_speed(rotation_speed),
		position(position)
	{
	}
};
}
}
}

#endif
