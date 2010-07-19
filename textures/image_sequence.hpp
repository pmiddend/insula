#ifndef INSULA_TEXTURES_IMAGE_SEQUENCE_HPP_INCLUDED
#define INSULA_TEXTURES_IMAGE_SEQUENCE_HPP_INCLUDED

#include <sge/image/file_ptr.hpp>
#include <vector>

namespace insula
{
namespace textures
{
typedef
std::vector
<
	sge::image::file_ptr
>
image_sequence;
}
}

#endif
