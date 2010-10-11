#ifndef INSULA_GIZMO_NORMALIZE_HPP_INCLUDED
#define INSULA_GIZMO_NORMALIZE_HPP_INCLUDED

#include "size_type.hpp"
#include "basic.hpp"
#include <algorithm>

namespace insula
{
namespace gizmo
{
template<typename T,size_type N>
basic<T,N> const
normalize(
	basic<T,N> b)
{
	std::transform(
		b.array().begin(),
		b.array().end(),
		b.array().begin(),
		[](typename basic<T,N>::vector const &v) { return normalize(v); });
	return b;
}
}
}

#endif
