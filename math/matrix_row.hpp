#ifndef INSULA_MATH_MATRIX_ROW_HPP_INCLUDED
#define INSULA_MATH_MATRIX_ROW_HPP_INCLUDED

#include <fcppt/math/matrix/matrix.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/size_type.hpp>

namespace insula
{
namespace math
{
template<typename T>
typename 
fcppt::math::vector::static_<T,4>::type const
matrix_row(
	typename fcppt::math::matrix::static_<T,4,4>::type const &m,
	fcppt::math::size_type const row)
{
	return 
		typename fcppt::math::vector::static_<T,4>::type(
			m[row][0],
			m[row][1],
			m[row][2],
			m[row][3]);
}
}
}

#endif
