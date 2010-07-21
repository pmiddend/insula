#ifndef INSULA_HEIGHT_MAP_ARRAY_TO_IMAGE_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_ARRAY_TO_IMAGE_HPP_INCLUDED

#include "array.hpp"
#include <sge/image/multi_loader.hpp>
#include <sge/image/file_ptr.hpp>
#include <fcppt/filesystem/path.hpp>

namespace insula
{
namespace height_map
{
void
array_to_image(
	array const &,
	sge::image::multi_loader &,
	fcppt::filesystem::path const &);
}
}

#endif
