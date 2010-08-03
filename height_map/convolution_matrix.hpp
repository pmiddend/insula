#ifndef INSULA_HEIGHT_MAP_CONVOLUTION_MATRIX_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_CONVOLUTION_MATRIX_HPP_INCLUDED

#include "scalar.hpp"
#include <fcppt/math/matrix/static.hpp>

namespace insula
{
namespace height_map
{
typedef 
fcppt::math::matrix::static_
<
	scalar,
	3,
	3
>::type
convolution_matrix;
}
}

#endif
