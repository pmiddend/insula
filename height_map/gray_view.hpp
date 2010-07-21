#ifndef INSULA_HEIGHT_MAP_GRAY_VIEW_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_GRAY_VIEW_HPP_INCLUDED

#include <mizuiro/image/const_view.hpp>
#include <sge/image/view/detail/view_types.hpp>
#include <sge/image/color/gray8_format.hpp>

namespace insula
{
namespace height_map
{
typedef
mizuiro::image::const_view
<
	// NOTE: Damn, why do I have to grab to ::detail here?
	sge::image::view::detail::view_types
	<
		sge::image::color::gray8_format
	// NOTE: The double ::type here are both needed!
	>::type
>::type
gray_view;
}
}

#endif
