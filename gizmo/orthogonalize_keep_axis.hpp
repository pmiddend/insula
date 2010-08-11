#ifndef INSULA_GIZMO_ORTHOGONALIZE_KEEP_AXIS_HPP_INCLUDED
#define INSULA_GIZMO_ORTHOGONALIZE_KEEP_AXIS_HPP_INCLUDED

#include "size_type.hpp"
#include <fcppt/math/vector/cross.hpp>

namespace insula
{
namespace gizmo
{
template<typename T,size_type N>
basic<T,N> const
orthogonalize_keep_axis(
	basic<T,N> g,
	size_type const axis)
{
	typedef typename 
	basic<T,N>::vector
	vector;

	size_type const 
		axisn = 
			static_cast<size_type>(axis + 1) % 
			static_cast<size_type>(3),
		axisnn = 
			static_cast<size_type>(axisn + 1) % 
			static_cast<size_type>(3);

	typename basic<T,N>::array_type &a = 
		g.array();

	a[axisnn] = 
		cross(
			a[axis],
			a[axisn]);

	a[axisn] = 
		cross(
			a[axisnn],
			a[axis]);

	return g;
}
}
}

#endif
