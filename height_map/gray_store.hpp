#ifndef INSULA_HEIGHT_MAP_GRAY_STORE_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_GRAY_STORE_HPP_INCLUDED

#include "gray_view.hpp"
#include <mizuiro/image/store.hpp>

namespace insula
{
namespace height_map
{
typedef
mizuiro::image::store
<
	gray_view::format,
	gray_view::access
>
gray_store;
}
}

#endif
