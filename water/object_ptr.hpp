#ifndef INSULA_WATER_OBJECT_PTR_HPP_INCLUDED
#define INSULA_WATER_OBJECT_PTR_HPP_INCLUDED

#include "object_fwd.hpp"
#include <memory>

namespace insula
{
namespace water
{
typedef
std::shared_ptr<object>
object_ptr;
}
}

#endif
