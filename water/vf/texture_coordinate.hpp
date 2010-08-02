#ifndef INSULA_WATER_VF_TEXTURE_COORDINATE_HPP_INCLUDED
#define INSULA_WATER_VF_TEXTURE_COORDINATE_HPP_INCLUDED

#include "../../graphics/scalar.hpp"
#include <sge/renderer/vf/make_unspecified_tag.hpp>
#include <sge/renderer/vf/unspecified.hpp>
#include <sge/renderer/vf/vector.hpp>

namespace insula
{
namespace water
{
namespace vf
{
namespace tags
{
SGE_RENDERER_VF_MAKE_UNSPECIFIED_TAG(texture_coordinate)
}

typedef 
sge::renderer::vf::unspecified
<
	sge::renderer::vf::vector
	<
		graphics::scalar,
		2
	>,
	tags::texture_coordinate
> 
texture_coordinate;
}
}
}

#endif
