#ifndef INSULA_MATH_PLANE_FROM_VEC4_HPP_INCLUDED
#define INSULA_MATH_PLANE_FROM_VEC4_HPP_INCLUDED

#include "basic.hpp"
#include <fcppt/math/vector/vector.hpp>

namespace insula
{
namespace math
{
namespace plane
{
template<typename T>
basic<T> const
from_vec4(
	typename fcppt::math::vector::static_<T,4>::type const &v)
{
	return 
		basic<T>(
			typename basic<T>::vector(
				v[0],
				v[1],
				v[2]),
			v[3]);
}
}
}
}

#endif
