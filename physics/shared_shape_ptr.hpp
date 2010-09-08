#ifndef INSULA_PHYSICS_SHARED_SHAPE_PTR_HPP_INCLUDED
#define INSULA_PHYSICS_SHARED_SHAPE_PTR_HPP_INCLUDED

#include <memory>

class btCollisionShape;

namespace insula
{
namespace physics
{
typedef
std::shared_ptr<btCollisionShape>
shared_shape_ptr;
}
}

#endif
