#ifndef INSULA_GIZMO_FROM_MAT3_HPP_INCLUDED
#define INSULA_GIZMO_FROM_MAT3_HPP_INCLUDED

#include <fcppt/math/matrix/static.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>

namespace insula
{
namespace gizmo
{
template<typename T>
basic<T,3> const
from_mat3(
	typename fcppt::math::matrix::static_<T,3,3>::type const &m)
{
	typedef typename
	basic<T,3>::vector
	vector;

	return 
		typename basic<T,3>::init()
			.position(
				vector::null())
			.right(
				vector(
					m[0][0],
					m[1][0],
					m[2][0]))
			.up(
				vector(
					m[0][1],
					m[1][1],
					m[2][1]))
			.forward(
				vector(
					m[0][2],
					m[1][2],
					m[2][2]));
}
}
}

#endif
