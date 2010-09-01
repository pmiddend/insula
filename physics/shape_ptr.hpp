#ifndef INSULA_PHYSICS_SHAPE_PTR_HPP_INCLUDED
#define INSULA_PHYSICS_SHAPE_PTR_HPP_INCLUDED

#include <memory>

class btCollisionShape;

namespace insula
{
namespace physics
{
typedef
std::shared_ptr<btCollisionShape>
shape_ptr;
}
}

#endif
