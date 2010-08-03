#include "average_convolute.hpp"
#include "convolute.hpp"
#include "convolution_matrix.hpp"
#include <fcppt/math/matrix/basic_impl.hpp>

insula::height_map::array const
insula::height_map::average_convolute(
	array const &a)
{
	scalar const cv = 
		static_cast<scalar>(1)/
		static_cast<scalar>(9);

	return 
		convolute(
			a,
			convolution_matrix(
				cv,cv,cv,
				cv,cv,cv,
				cv,cv,cv));
}
