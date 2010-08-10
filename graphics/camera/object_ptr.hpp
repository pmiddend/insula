#ifndef INSULA_GRAPHICS_CAMERA_OBJECT_PTR_HPP_INCLUDED
#define INSULA_GRAPHICS_CAMERA_OBJECT_PTR_HPP_INCLUDED

#include "object_fwd.hpp"
#include <memory>

namespace insula
{
namespace graphics
{
namespace camera
{
typedef
std::shared_ptr<object>
object_ptr;
}
}
}

#endif
