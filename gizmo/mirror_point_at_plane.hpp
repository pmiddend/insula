#ifndef INSULA_GIZMO_MIRROR_POINT_AT_PLANE_HPP_INCLUDED
#define INSULA_GIZMO_MIRROR_POINT_AT_PLANE_HPP_INCLUDED

#include <fcppt/math/vector/static.hpp>
#include <fcppt/math/vector/basic_impl.hpp>

namespace insula
{
namespace gizmo
{
template<typename T>
typename
fcppt::math::vector::static_<T,3>::type const
mirror_point_at_plane(
	typename fcppt::math::vector::static_<T,3>::type const &v,
	T const plane_height)
{
	return 
		typename fcppt::math::vector::static_<T,3>::type(
			v.x(),
			-v.y() + static_cast<T>(2) * plane_height,
			v.z());
}
}
}

#endif
