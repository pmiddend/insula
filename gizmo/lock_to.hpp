#ifndef INSULA_GIZMO_LOCK_TO_HPP_INCLUDED
#define INSULA_GIZMO_LOCK_TO_HPP_INCLUDED

#include "basic.hpp"
#include "orthogonalize_keep_axis.hpp"
#include "normalize.hpp"
#include <fcppt/math/matrix/rotation_axis.hpp>
#include <fcppt/math/matrix/matrix.hpp>

namespace insula
{
namespace gizmo
{
template<typename T>
basic<T,3> const
lock_to(
	basic<T,3> g,
	T const distance,
	T const angle)
{
	typedef typename
	basic<T,3>::vector
	vector;

	vector const rotation_axis = 
		g.right();

	typename fcppt::math::matrix::static_<T,4,4>::type const rotation_matrix = 
		fcppt::math::matrix::rotation_axis(
			angle,
			rotation_axis);

	vector const old_position = g.position();

	// This seems to work
	g.position( 
 		(g.position() + 
		fcppt::math::vector::narrow_cast<vector>(
			rotation_matrix * 
			fcppt::math::vector::construct(
				-distance * g.forward(),
				static_cast<T>(
					0)))));

	g.forward(
		normalize(
			//old_position - g.position()));
			g.position() - old_position));
	g.up(
		vector(0,1,0));
	g.right(
		cross(
			g.up(),g.forward()));
	g.up(
		cross(
			g.forward(),
			g.right()));

	return 
		normalize(
			g);
	/*
	std::transform(
		g.array().begin(),
		g.array().end(),
		g.array().begin(),
		[&rotation_matrix](typename basic<T,3>::vector const &v) 
		{ 
			return 
				fcppt::math::vector::narrow_cast<vector>(
					rotation_matrix * 
					fcppt::math::vector::construct(
						v,
						static_cast<T>(
							0)));
		});
	*/

	/*
	return 
		normalize(
			orthogonalize_keep_axis(
				g,
				2));*/
}
}
}

#endif
