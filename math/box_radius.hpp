#ifndef INSULA_MATH_BOX_RADIUS_HPP_INCLUDED
#define INSULA_MATH_BOX_RADIUS_HPP_INCLUDED

#include <fcppt/math/size_type.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/dim/arithmetic.hpp>

namespace insula
{
namespace math
{
template<typename T,fcppt::math::size_type N>
T
box_radius(
	fcppt::math::box::basic<T,N> const &b)
{
	return 
		fcppt::math::vector::length(
			fcppt::math::dim::structure_cast
			<
				typename fcppt::math::box::basic<T,N>::vector
			>(
				b.dimension()/
				static_cast<T>(2)));
}
}
}

#endif
