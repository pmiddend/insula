#ifndef INSULA_HEIGHT_MAP_DIM2_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_DIM2_HPP_INCLUDED

#include "scalar.hpp"
#include <fcppt/math/dim/static.hpp>

namespace insula
{
namespace height_map
{
typedef
fcppt::math::dim::static_
<
	scalar,
	2
>::type
dim2;
}
}

#endif
