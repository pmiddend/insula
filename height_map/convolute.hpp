#ifndef INSULA_HEIGHT_MAP_CONVOLUTE_HPP_INCLDUDED
#define INSULA_HEIGHT_MAP_CONVOLUTE_HPP_INCLDUDED

#include "array.hpp"
#include "convolution_matrix.hpp"

namespace insula
{
namespace height_map
{
array const 
convolute(
	array const &,
	convolution_matrix const &);
}
}

#endif
