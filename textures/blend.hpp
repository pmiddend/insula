#ifndef INSULA_TEXTURES_BLEND_HPP_INCLUDED
#define INSULA_TEXTURES_BLEND_HPP_INCLUDED

#include "image_sequence.hpp"
#include "rgb_store.hpp"
#include "interpolators/base.hpp"
#include "../height_map/array.hpp"
#include <sge/image/file_ptr.hpp>

namespace insula
{
namespace textures
{
rgb_store const
blend(
	image_sequence const &,
	height_map::array const &,
	interpolators::base &);
}
}

#endif
