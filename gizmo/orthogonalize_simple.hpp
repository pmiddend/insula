#ifndef INSULA_GIZMO_ORTHOGONALIZE_SIMPLE_HPP_INCLUDED
#define INSULA_GIZMO_ORTHOGONALIZE_SIMPLE_HPP_INCLUDED

#include "size_type.hpp"

namespace insula
{
namespace gizmo
{
template<typename T,size_type N>
basic<T,N> const
orthogonalize_simple(
	basic<T,N> g)
{
	typedef typename 
	basic<T,N>::vector
	vector;
	
	// -Construct a new right. 
	// -This right is orthogonal to up and forward. 
	// -But up and forward are not neccessarily orthgonal.
	// -Say right and up are orthogonal and forward is "broken"
	// -Construct new forward from dot product of right and up
	g.right(
		cross(
			g.up(),
			g.forward()));
	g.forward(
		cross(
			g.right(),	
			g.up()));

	return g;
}
}
}

#endif
