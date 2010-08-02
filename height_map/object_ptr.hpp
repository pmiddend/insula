#ifndef INSULA_HEIGHT_MAP_OBJECT_PTR_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_OBJECT_PTR_HPP_INCLUDED

#include "object_fwd.hpp"
#include <fcppt/shared_ptr.hpp>

namespace insula
{
namespace height_map
{
typedef
fcppt::shared_ptr<object>
object_ptr;
}
}

#endif
