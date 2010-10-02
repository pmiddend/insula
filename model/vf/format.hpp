#ifndef INSULA_MODEL_VF_FORMAT_HPP_INCLUDED
#define INSULA_MODEL_VF_FORMAT_HPP_INCLUDED

#include "position.hpp"
#include "texcoord.hpp"
#include "normal.hpp"
#include <sge/renderer/vf/format.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace insula
{
namespace model
{
namespace vf
{
typedef 
sge::renderer::vf::format
<
	boost::mpl::vector3
	<
		position,
		texcoord,
		normal
	>
> 
format;
}
}
}

#endif
