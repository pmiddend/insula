#ifndef INSULA_MATH_BARYCENTRIC_COORDINATES_HPP_INCLUDED
#define INSULA_MATH_BARYCENTRIC_COORDINATES_HPP_INCLUDED

#include <fcppt/math/vector/cross.hpp>
#include <fcppt/math/vector/length_square.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <cmath>

namespace insula
{
namespace math
{
// NOTE: This could be abstracted for cases where N != 3, see
// http://en.wikipedia.org/wiki/Barycentric_coordinates_(mathematics)
template<typename T>
typename
fcppt::math::vector::static_<T,3>::type const
barycentric_coordinates(
	typename fcppt::math::vector::static_<T,3>::type const &position,
	typename fcppt::math::vector::static_<T,3>::type const &p1,
	typename fcppt::math::vector::static_<T,3>::type const &p2,
	typename fcppt::math::vector::static_<T,3>::type const &p3)
{
	typedef typename 
	fcppt::math::vector::static_<T,3>::type 
	vector;

	vector const edge1 = 
		p2 - p1;

	vector const edge2 = 
		p3 - p1;

	// Area of triangle ABC
	T const p1p2p3 = 
		fcppt::math::vector::length_square(
			fcppt::math::vector::cross(
				edge1,
				edge2));

	// Area of BCP
	T const p2p3p = 
		fcppt::math::vector::length_square(
			fcppt::math::vector::cross(
				p3 - p2,
				position - p2));

	// Area of CAP
	T const p3p1p = 
		fcppt::math::vector::length_square(
			fcppt::math::vector::cross(
				edge2,
				position - p3));

	T const s = std::sqrt(p2p3p / p1p2p3);
	T const t = std::sqrt(p3p1p / p1p2p3);
	T const w = static_cast<T>(1) - s - t;

	return 
		vector(s,t,w);
}
}
}

#endif
