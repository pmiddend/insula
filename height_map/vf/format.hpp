#ifndef INSULA_HEIGHT_MAP_VF_FORMAT_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_VF_FORMAT_HPP_INCLUDED

#include "position.hpp"
#include "normal.hpp"
#include "texcoord.hpp"
#include <sge/renderer/vf/format.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace insula
{
namespace height_map
{
namespace vf
{
typedef 
sge::renderer::vf::format
<
	boost::mpl::vector3
	<
		position,
		normal,
		texcoord
	>
> 
format;
}
}
}

#endif
