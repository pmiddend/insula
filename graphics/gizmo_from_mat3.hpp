#ifndef INSULA_GRAPHICS_GIZMO_FROM_MAT3_HPP_INCLUDED
#define INSULA_GRAPHICS_GIZMO_FROM_MAT3_HPP_INCLUDED

#include "mat3.hpp"
#include "basic_gizmo.hpp"
#include <fcppt/math/matrix/basic_impl.hpp>

namespace insula
{
namespace graphics
{
template<typename T>
basic_gizmo<T> const
gizmo_from_mat3(
	mat3 const &m)
{
	return 
		typename basic_gizmo<T>::init()
			.right(
				T(
					m[0][0],
					m[1][0],
					m[2][0]))
			.up(
				T(
					m[0][1],
					m[1][1],
					m[2][1]))
			.forward(
				T(
					m[0][2],
					m[1][2],
					m[2][2]));

}
}
}

#endif
