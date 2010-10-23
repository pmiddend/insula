#ifndef INSULA_GIZMO_TO_MAT4_HPP_INCLUDED
#define INSULA_GIZMO_TO_MAT4_HPP_INCLUDED

#include "rotation_to_mat4.hpp"
#include "basic.hpp"
#include <fcppt/math/matrix/translation.hpp>

namespace insula
{
namespace gizmo
{
template<typename T>
typename
fcppt::math::matrix::static_<T,4,4>::type
to_mat4(
	gizmo::basic<T,3> const &g)
{
	return 
		rotation_to_mat4(g) * 
		fcppt::math::matrix::translation(
			-g.position());
}
}
}

#endif
