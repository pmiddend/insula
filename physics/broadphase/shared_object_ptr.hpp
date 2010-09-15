#ifndef INSULA_PHYSICS_BROADPHASE_SHARED_OBJECT_PTR_HPP_INCLUDED
#define INSULA_PHYSICS_BROADPHASE_SHARED_OBJECT_PTR_HPP_INCLUDED

// We include the full intrusive wrapper here because we don't want
// the user to see the wrapper and rather think that it's the
// btDbvtBroadphase
#include "intrusive_wrapper.hpp"
#include <memory>

namespace insula
{
namespace physics
{
namespace broadphase
{
typedef
std::shared_ptr<intrusive_wrapper>
shared_object_ptr;
}
}
}

#endif
