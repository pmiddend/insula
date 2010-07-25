#ifndef INSULA_HEIGHT_MAP_VF_HEIGHT_AND_GRADIENT_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_VF_HEIGHT_AND_GRADIENT_HPP_INCLUDED

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
SGE_RENDERER_VF_MAKE_UNSPECIFIED_TAG(height_and_gradient)
}

typedef 
sge::renderer::vf::unspecified
<
	sge::renderer::vf::vector
	<
		graphics::scalar,
		2
	>,
	tags::height_and_gradient
> 
height_and_gradient;
}
}
}

#endif
