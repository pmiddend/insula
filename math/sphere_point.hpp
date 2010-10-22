#ifndef INSULA_MATH_SPHERE_POINT_HPP_INCLUDED
#define INSULA_MATH_SPHERE_POINT_HPP_INCLUDED

#include <fcppt/math/vector/static.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <cmath>

namespace insula
{
namespace math
{
/// Returns the point on the sphere designated by the two angles "lat"
/// and "lon". lat corresponds to the latitude and lon the longitude,
/// respectively.
///
/// Examples
///
/// Let's assume radius is 1, then: 
///
/// sphere_point(0,0) =          (0,1,0)
/// sphere_point(90 degree,0) =  (1,0,0)
/// sphere_point(180 degree,0) = (0,-1,0)
/// sphere_point(-90 degree,0) = (-1,0,0)
///
/// So the latitude starts (=0) at the tip of the sphere and goes down
/// to the right with increasing angle.
template<typename T>
typename
fcppt::math::vector::static_<T,3>::type const
sphere_point(
	T const radius,
	T const lat,
	T const lon)
{
	using std::sin;
	using std::cos;

	return 
		radius * 
		typename fcppt::math::vector::static_<T,3>::type(
			cos(lon) * sin(lat),
			cos(lat),
			sin(lon) * sin(lat));
}
}
}

#endif
