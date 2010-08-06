#ifndef INSULA_PHYSICS_VF_POSITION_HPP_INCLUDED
#define INSULA_PHYSICS_VF_POSITION_HPP_INCLUDED

#include "../../graphics/scalar.hpp"
#include <sge/renderer/vf/make_unspecified_tag.hpp>
#include <sge/renderer/vf/unspecified.hpp>
#include <sge/renderer/vf/vector.hpp>

namespace insula
{
namespace physics
{
namespace vf
{
namespace tags
{
SGE_RENDERER_VF_MAKE_UNSPECIFIED_TAG(position)
}

typedef 
sge::renderer::vf::unspecified
<
	sge::renderer::vf::vector
	<
		graphics::scalar,
		3
	>,
	tags::position
> 
position;
}
}
}

#endif
