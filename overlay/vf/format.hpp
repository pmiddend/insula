#ifndef INSULA_OVERLAY_VF_FORMAT_HPP_INCLUDED
#define INSULA_OVERLAY_VF_FORMAT_HPP_INCLUDED

#include "position.hpp"
#include <sge/renderer/vf/format.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace insula
{
namespace overlay
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
