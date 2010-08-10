#ifndef INSULA_GIZMO_STRUCTURE_CAST_HPP_INCLUDED
#define INSULA_GIZMO_STRUCTURE_CAST_HPP_INCLUDED

#include <fcppt/math/vector/structure_cast.hpp>

namespace insula
{
namespace gizmo
{
template<typename Dest,typename Source>
Dest const
structure_cast(
	Source const &s)
{
	return 
		typename Dest::init()
			.position(
				fcppt::math::vector::structure_cast<typename Dest::vector>(
					s.position()))
			.right(
				fcppt::math::vector::structure_cast<typename Dest::vector>(
					s.right()))
			.up(
				fcppt::math::vector::structure_cast<typename Dest::vector>(
					s.up()))
			.forward(
				fcppt::math::vector::structure_cast<typename Dest::vector>(
					s.forward()));
}
}
}

#endif
