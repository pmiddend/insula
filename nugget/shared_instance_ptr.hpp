#ifndef INSULA_NUGGET_SHARED_INSTANCE_PTR_HPP_INCLUDED
#define INSULA_NUGGET_SHARED_INSTANCE_PTR_HPP_INCLUDED

#include "instance_fwd.hpp"
#include <memory>

namespace insula
{
namespace nugget
{
typedef
std::shared_ptr<instance>
shared_instance_ptr;
}
}

#endif
