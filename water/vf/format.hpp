#ifndef INSULA_WATER_VF_FORMAT_HPP_INCLUDED
#define INSULA_WATER_VF_FORMAT_HPP_INCLUDED

#include "position.hpp"
#include "texture_coordinate.hpp"
#include <sge/renderer/vf/format.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace insula
{
namespace water
{
namespace vf
{
typedef 
sge::renderer::vf::format
<
	boost::mpl::vector2
	<
		position,
		texture_coordinate
	>
> 
format;
}
}
}

#endif
