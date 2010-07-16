#ifndef INSULA_HEIGHT_MAP_ARRAY_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_ARRAY_HPP_INCLUDED

#include "scalar.hpp"
#include <boost/multi_array.hpp>

namespace insula
{
namespace height_map
{
typedef
boost::multi_array
<
	scalar,
	2
>
array;
}
}

#endif
