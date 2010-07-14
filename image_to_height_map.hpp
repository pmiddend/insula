#ifndef INSULA_IMAGE_TO_HEIGHT_MAP_HPP_INCLUDED
#define INSULA_IMAGE_TO_HEIGHT_MAP_HPP_INCLUDED

#include "height_map.hpp"
#include <sge/image/file_ptr.hpp>

namespace insula
{
height_map const
image_to_height_map(
	sge::image::file_ptr);
}

#endif
