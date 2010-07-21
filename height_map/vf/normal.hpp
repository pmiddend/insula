#ifndef INSULA_HEIGHT_MAP_NORMAL_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_NORMAL_HPP_INCLUDED

#include "../../graphics/scalar.hpp"
#include <sge/renderer/vf/unspecified.hpp>

namespace insula
{
namespace height_map
{
namespace vf
{
typedef 
sge::renderer::vf::unspecified
<
	graphics::scalar,
	3
> 
normal;
}
}
}

#endif
