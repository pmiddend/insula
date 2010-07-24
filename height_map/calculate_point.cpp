#include "calculate_point.hpp"
#include <fcppt/math/vector/basic_impl.hpp>
#include <type_traits>
#include <algorithm>

insula::height_map::vf::packed_normal const 
insula::height_map::calculate_point(
	array const &heights,
	scalar const height_scaling,
	vec2 const &cell_sizes,
	std::make_signed<array::size_type>::type rx,
	std::make_signed<array::size_type>::type ry)
{
	array::size_type const 
		x = 
			static_cast<array::size_type>(
				std::max(
					static_cast<std::make_signed<array::size_type>::type>(0),
					std::min(
						static_cast<std::make_signed<array::size_type>::type>(
							heights.shape()[0]-1),
						rx))),
		y = 
			static_cast<array::size_type>(
				std::max(
					static_cast<std::make_signed<array::size_type>::type>(0),
					std::min(
						static_cast<std::make_signed<array::size_type>::type>(
							heights.shape()[1]-1),
						ry)));
	return 
		vf::packed_normal(
			static_cast<insula::graphics::scalar>(
					static_cast<scalar>(
						x) * 
					cell_sizes.x()),
				static_cast<insula::graphics::scalar>(
					static_cast<scalar>(
						height_scaling) * 
					heights[y][x]),
				static_cast<insula::graphics::scalar>(
					static_cast<scalar>(
						y) * 
					cell_sizes.y()));
}
