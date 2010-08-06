#ifndef INSULA_PHYSICS_VF_FORMAT_HPP_INCLUDED
#define INSULA_PHYSICS_VF_FORMAT_HPP_INCLUDED

#include "position.hpp"
#include <sge/renderer/vf/format.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace insula
{
namespace physics
{
namespace vf
{
typedef 
sge::renderer::vf::format
<
	boost::mpl::vector1
	<
		position
	>
> 
format;
}
}
}

#endif
