#ifndef INSULA_HEIGHT_MAP_DIM3_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_DIM3_HPP_INCLUDED

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
	3
>::type
dim3;
}
}

#endif
