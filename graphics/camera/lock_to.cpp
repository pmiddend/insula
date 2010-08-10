#include "lock_to.hpp"
#include "object.hpp"
#include "../scalar.hpp"
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/construct.hpp>
#include <fcppt/math/vector/narrow_cast.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/vector.hpp>
#include <fcppt/math/matrix/rotation_axis.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>

void
insula::graphics::camera::lock_to(
	object &c,
	gizmo const &g,
	scalar const distance,
	scalar const angle)
{
	c.gizmo().position( 
 		-(g.position() + 
		fcppt::math::vector::narrow_cast<vec3>(
			fcppt::math::matrix::rotation_axis(
				angle,
				g.right()) * 
			fcppt::math::vector::construct(
				-distance * g.forward(),
				static_cast<scalar>(
					0)))));
}
