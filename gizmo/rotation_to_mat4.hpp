#ifndef INSULA_GIZMO_ROTATION_TO_MAT4_HPP_INCLUDED
#define INSULA_GIZMO_ROTATION_TO_MAT4_HPP_INCLUDED

#include "size_type.hpp"
#include <fcppt/math/matrix/static.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>

namespace insula
{
namespace gizmo
{
template<typename T,size_type N>
typename 
fcppt::math::matrix::static_<T,4,4>::type const
rotation_to_mat4(
	basic<T,N> const &b)
{
	typedef
	T
	scalar;

	typedef typename
	basic<T,N>::array_type
	array_type;

	scalar const 
		zero = 
			static_cast<scalar>(0),
		one = 
			static_cast<scalar>(1);

	array_type const &a = 
		b.array();

	return 
		// NOTE that transposing here is correct!
		typename fcppt::math::matrix::static_<scalar,4,4>::type(
			a[0][0],a[0][1],a[0][2],zero,
			a[1][0],a[1][1],a[1][2],zero,
			a[2][0],a[2][1],a[2][2],zero,
			zero,zero,zero,one);
}
}
}

#endif
