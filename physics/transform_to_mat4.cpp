#include "transform_to_mat4.hpp"
#include "bullet_to_vec3.hpp"
#include "bullet_to_mat4.hpp"
#include <LinearMath/btTransform.h>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>

insula::graphics::mat4 const
insula::physics::transform_to_mat4(
	btTransform const &t)
{
	return 
		fcppt::math::matrix::translation(
				bullet_to_vec3(
					t.getOrigin())) * 
			bullet_to_mat4(
				t.getBasis());
}
