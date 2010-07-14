#ifndef INSULA_HEIGHT_MAP_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_HPP_INCLUDED

#include <boost/multi_array.hpp>

namespace insula
{
typedef
boost::multi_array
<
	float,
	2
>
height_map;
}

#endif
