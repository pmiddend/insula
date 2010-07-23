#include "calculate_normal.hpp"
#include "calculate_point.hpp"
#include "../graphics/scalar.hpp"
#include <fcppt/math/vector/cross.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <type_traits>
#include <algorithm>

insula::height_map::vf::packed_normal const 
insula::height_map::calculate_normal(
	array const &heights,
	scalar const height_scaling,
	vector2 const &cell_sizes,
	array::size_type const x,
	array::size_type const y)
{
	insula::height_map::vf::packed_normal const
		point = calculate_point(heights,height_scaling,cell_sizes,x,y),
		left = calculate_point(heights,height_scaling,cell_sizes,x-1,y),
		right = calculate_point(heights,height_scaling,cell_sizes,x+1,y),
		top = calculate_point(heights,height_scaling,cell_sizes,x,y-1),
		bottom = calculate_point(heights,height_scaling,cell_sizes,x,y+1),
		to_top = top - point,
		to_right = right - point,
		to_bottom = bottom - point,
		to_left = left - point;

	return 
		(fcppt::math::vector::cross(
			to_right,
			to_top) + 
		fcppt::math::vector::cross(
			to_top,
			to_left) + 
		fcppt::math::vector::cross(
			to_left,
			to_bottom) + 
		fcppt::math::vector::cross(
			to_bottom,
			to_right))/static_cast<insula::graphics::scalar>(4);
}
