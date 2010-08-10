#include "gizmo_from_mat3.hpp"
#include "vec3.hpp"
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/vector/basic_impl.hpp>

insula::graphics::gizmo const
insula::graphics::gizmo_from_mat3(
	mat3 const &m)
{
	return 
		gizmo_init()
			.right(
				vec3(
					m[0][0],
					m[1][0],
					m[2][0]))
			.up(
				vec3(
					m[0][1],
					m[1][1],
					m[2][1]))
			.forward(
				vec3(
					m[0][2],
					m[1][2],
					m[2][2]));
}
