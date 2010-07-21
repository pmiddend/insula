#include "generate_gradient.hpp"
#include <array>
#include <cmath>
#include <algorithm>

insula::height_map::array const
insula::height_map::generate_gradient(
	array const &a)
{
	array result(
		boost::extents[a.shape()[0]][a.shape()[1]]);

	for (array::size_type y = 1; y < a.shape()[0]-1; ++y)
	{
		for (array::size_type x = 1; x < a.shape()[1]-1; ++x)
		{
			scalar const current = a[y][x];

			std::array<scalar,8> neighbors = {{
				a[y][x-1],
				a[y][x+1],
				a[y-1][x],
				a[y+1][x],
				a[y-1][x-1],
				a[y-1][x+1],
				a[y+1][x-1],
				a[y+1][x+1]
				}};

			std::transform(
				neighbors.begin(),
				neighbors.end(),
				neighbors.begin(),
				[&current](scalar const s) { return std::abs(current - s); });

			result[y][x] = 
				*std::max_element(
					neighbors.begin(),
					neighbors.end());
		}
	}
	return result;
}
