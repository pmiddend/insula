#ifndef INSULA_MATH_PLANE_NORMALIZE_HPP_INCLUDED
#define INSULA_MATH_PLANE_NORMALIZE_HPP_INCLUDED

#include "basic.hpp"
#include <fcppt/math/vector/normalize.hpp>

namespace insula
{
namespace math
{
namespace plane
{
template<typename T>
basic<T> const
normalize(
	basic<T> const &p)
{
	return 
		basic<T>(
			fcppt::math::vector::normalize(
				p.normal()),
			p.lambda()/
			fcppt::math::vector::length(
				p.normal()));
}
}
}
}

#endif
