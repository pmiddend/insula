#ifndef INSULA_GRAPHICS_SHADER_SHARED_OBJECT_PTR_HPP_INCLUDED
#define INSULA_GRAPHICS_SHADER_SHARED_OBJECT_PTR_HPP_INCLUDED

#include "object_fwd.hpp"
#include <memory>

namespace insula
{
namespace graphics
{
namespace shader
{
typedef
std::shared_ptr<object>
shared_object_ptr;
}
}
}

#endif
