#ifndef INSULA_MATH_EXTRACT_FRUSTUM_HPP_INCLUDED
#define INSULA_MATH_EXTRACT_FRUSTUM_HPP_INCLUDED

#include "basic_frustum.hpp"
#include "plane/from_vec4.hpp"
#include "plane/normalize.hpp"
#include "matrix_row.hpp"
#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/math/vector/vector.hpp>

namespace insula
{
namespace math
{
// See http://www.cs.otago.ac.nz/postgrads/alexis/planeExtraction.pdf
template<typename T>
basic_frustum<T> const
extract_frustum(
	typename fcppt::math::matrix::static_<T,4,4>::type const &m)
{
	basic_frustum<T> result;
	result.left(plane::normalize(plane::from_vec4<T>(matrix_row<T>(m,3) + matrix_row<T>(m,0))));
	result.right(plane::normalize(plane::from_vec4<T>(matrix_row<T>(m,3) - matrix_row<T>(m,0))));
	result.bottom(plane::normalize(plane::from_vec4<T>(matrix_row<T>(m,3) + matrix_row<T>(m,1))));
	result.top(plane::normalize(plane::from_vec4<T>(matrix_row<T>(m,3) - matrix_row<T>(m,1))));
	result.near(plane::normalize(plane::from_vec4<T>(matrix_row<T>(m,3) + matrix_row<T>(m,2))));
	result.far(plane::normalize(plane::from_vec4<T>(matrix_row<T>(m,3) - matrix_row<T>(m,2))));
	return result;
}
}
}

#endif
