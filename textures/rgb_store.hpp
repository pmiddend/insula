#ifndef INSULA_TEXTURES_RGB_STORE_HPP_INCLUDED
#define INSULA_TEXTURES_RGB_STORE_HPP_INCLUDED

#include "rgb_view.hpp"
#include <mizuiro/image/store.hpp>

namespace insula
{
namespace textures
{
typedef
mizuiro::image::store
<
	rgb_view::format,
	rgb_view::access
>
rgb_store;
}
}

#endif
