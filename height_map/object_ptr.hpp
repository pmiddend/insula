#ifndef INSULA_HEIGHT_MAP_OBJECT_PTR_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_OBJECT_PTR_HPP_INCLUDED

#include "object_fwd.hpp"
#include <memory>

namespace insula
{
namespace height_map
{
typedef
std::shared_ptr<object>
object_ptr;
}
}

#endif
