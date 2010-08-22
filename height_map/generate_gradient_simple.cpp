#include "generate_gradient_simple.hpp"
#include <fcppt/container/grid/object_impl.hpp>
#include <array>
#include <cmath>
#include <algorithm>

insula::height_map::array const
insula::height_map::generate_gradient_simple(
	array const &a)
{
	array result(
		a.dimension());

	for (array::size_type y = 1; y < a.dimension().h()-1; ++y)
	{
		for (array::size_type x = 1; x < a.dimension().w()-1; ++x)
		{
			scalar const current = a[array::dim(x,y)];

			std::array<scalar,8> neighbors = {{
				a[array::dim(x,y-1)],
				a[array::dim(x,y+1)],
				a[array::dim(x-1,y)],
				a[array::dim(x+1,y)],
				a[array::dim(x-1,y-1)],
				a[array::dim(x-1,y+1)],
				a[array::dim(x+1,y-1)],
				a[array::dim(x+1,y+1)]
				}};

			std::transform(
				neighbors.begin(),
				neighbors.end(),
				neighbors.begin(),
				[&current](scalar const s) { return std::abs(current - s); });

			result[array::dim(x,y)] = 
				*std::max_element(
					neighbors.begin(),
					neighbors.end());
		}
	}
	return result;
}
