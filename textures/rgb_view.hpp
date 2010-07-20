#ifndef INSULA_TEXTURES_RGB_VIEW_HPP_INCLUDED
#define INSULA_TEXTURES_RGB_VIEW_HPP_INCLUDED

#include <mizuiro/image/const_view.hpp>
#include <sge/image/view/detail/view_types.hpp>
#include <sge/image/color/rgb8_format.hpp>

namespace insula
{
namespace textures
{
typedef
mizuiro::image::const_view
<
	sge::image::view::detail::view_types
	<
		sge::image::color::rgb8_format
	>::type
>::type
rgb_view;
}
}

#endif
