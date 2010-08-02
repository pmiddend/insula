#ifndef INSULA_WATER_OBJECT_PTR_HPP_INCLUDED
#define INSULA_WATER_OBJECT_PTR_HPP_INCLUDED

#include "object_fwd.hpp"
#include <fcppt/shared_ptr.hpp>

namespace insula
{
namespace water
{
typedef
fcppt::shared_ptr<object>
object_ptr;
}
}

#endif
