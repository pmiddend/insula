#ifndef INSULA_TEXTURES_BLEND_HPP_INCLUDED
#define INSULA_TEXTURES_BLEND_HPP_INCLUDED

#include "rgb_view.hpp"
#include "rgb_view_sequence.hpp"
#include "rgb_store.hpp"
#include "interpolators/base.hpp"
#include "../height_map/array.hpp"

namespace insula
{
namespace textures
{
rgb_store const
blend(
	rgb_view const &gradient_image,
	rgb_view_sequence const &height_images,
	height_map::array const &heighs,
	height_map::array const &grad,
	interpolators::base &interpolator);
}
}

#endif
