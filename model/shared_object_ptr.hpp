#ifndef INSULA_MODEL_SHARED_OBJECT_PTR_HPP_INCLUDED
#define INSULA_MODEL_SHARED_OBJECT_PTR_HPP_INCLUDED

#include "object_fwd.hpp"
#include <memory>

namespace insula
{
namespace model
{
typedef
std::shared_ptr<object>
shared_object_ptr;
}
}

#endif
