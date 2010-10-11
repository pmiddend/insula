#ifndef INSULA_MATH_UNIFORM_SCALING_MATRIX_HPP_INCLUDED
#define INSULA_MATH_UNIFORM_SCALING_MATRIX_HPP_INCLUDED

#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/math/matrix/scaling.hpp>

namespace insula
{
namespace math
{
template<typename T>
typename 
fcppt::math::matrix::static_<T, 4, 4>::type const
uniform_scaling_matrix(
	T const p)
{
	return 
		fcppt::math::matrix::scaling(
			p,
			p,
			p);
}
}
}

#endif
