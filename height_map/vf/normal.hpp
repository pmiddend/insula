#ifndef INSULA_HEIGHT_MAP_NORMAL_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_NORMAL_HPP_INCLUDED

#include "../../graphics/scalar.hpp"
#include <sge/renderer/vf/make_unspecified_tag.hpp>
#include <sge/renderer/vf/unspecified.hpp>
#include <sge/renderer/vf/vector.hpp>

namespace insula
{
namespace height_map
{
namespace vf
{
namespace tags
{
SGE_RENDERER_VF_MAKE_UNSPECIFIED_TAG(normal)
}

typedef 
sge::renderer::vf::unspecified
<
	sge::renderer::vf::vector
	<
		graphics::scalar,
		3
	>,
	tags::normal
> 
normal;
}
}
}

#endif
