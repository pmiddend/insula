#ifndef INSULA_HEIGHT_MAP_ARRAY_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_ARRAY_HPP_INCLUDED

#include "scalar.hpp"
#include <fcppt/container/grid/object.hpp>

namespace insula
{
namespace height_map
{
typedef
fcppt::container::grid::object
<
	scalar,
	2
>
array;
}
}

#endif
