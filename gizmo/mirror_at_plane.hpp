#ifndef INSULA_GIZMO_MIRROR_AT_PLANE_HPP_INCLUDED
#define INSULA_GIZMO_MIRROR_AT_PLANE_HPP_INCLUDED

#include "mirror_point_at_plane.hpp"
#include "orthogonalize_simple.hpp"
#include "normalize.hpp"

// DEBUG BEGIN
//#include <iostream>
// DEBUG END

namespace insula
{
namespace gizmo
{
template<typename T>
basic<T,3> const
mirror_at_plane(
	basic<T,3> const &g,
	T const plane_height)
{
	typedef typename
	basic<T,3>::array_type
	array_type;

	// DEBUG BEGIN
	//std::cout << "old camera y pos: " << g.position()[1] << "\n";
	// DEBUG END

	basic<T,3> n(
		typename basic<T,3>::init()
			.position(
				mirror_point_at_plane<T>(
					g.position(),
					plane_height)));

	// DEBUG BEGIN
//	std::cout << "new camera y pos: " << n.position()[1] << "\n";
	// DEBUG END

	for (typename array_type::size_type i = 0; i < 3; ++i)
	{
		n.array()[i] = 
			mirror_point_at_plane<T>(
				g.position() + g.array()[i],
				plane_height) - 
			n.position();
	}

	return 
		normalize(
			orthogonalize_simple(
				n));
}
}
}

#endif
