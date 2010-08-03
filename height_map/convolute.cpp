#include "convolute.hpp"
#include <fcppt/math/matrix/basic_impl.hpp>

namespace
{
insula::height_map::scalar
convolute_cell(
	insula::height_map::array const &a,
	insula::height_map::convolution_matrix const &c,
	insula::height_map::array::size_type const cx,
	insula::height_map::array::size_type const cy)
{
	using namespace insula::height_map;

	scalar sum = 
		static_cast<scalar>(0);
	for (
		array::size_type x = cx; 
		x < static_cast<array::size_type>(cx+c.dimension()[0]); 
		++x)
		for (
			array::size_type y = cy; 
			y < static_cast<array::size_type>(cy+c.dimension()[1]); 
			++y)
		{
			sum += c[static_cast<convolution_matrix::size_type>(x-cx)][static_cast<convolution_matrix::size_type>(y-cy)] * a[x][y];
		}
	return sum;
}
}

insula::height_map::array const 
insula::height_map::convolute(
	array const &a,
	convolution_matrix const &c)
{
	convolution_matrix::size_type const 
		whalf = c.dimension()[0]/2,
		hhalf = c.dimension()[1]/2;

	//array result(boost::extents[a.shape()[0]][a.shape()[1]]);
	array result(
		a);

	for(
		array::size_type x = static_cast<array::size_type>(whalf); 
		x < static_cast<array::size_type>(a.shape()[0]-whalf);
		++x)
	{
		for(
			array::size_type y = static_cast<array::size_type>(hhalf); 
			y < static_cast<array::size_type>(a.shape()[1]-hhalf); 
			++y)
		{
			array::size_type 
				start_x = 
					static_cast<array::size_type>(x-whalf),
				start_y =
					static_cast<array::size_type>(y-hhalf);
				
			result[x][y] = 
				convolute_cell(
					a,
					c,
					start_x,start_y);
		}
	}
	
	return result;
}
