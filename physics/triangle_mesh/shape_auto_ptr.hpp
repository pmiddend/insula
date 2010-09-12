#ifndef INSULA_PHYSICS_TRIANGLE_MESH_SHAPE_AUTO_PTR_HPP_INCLUDED
#define INSULA_PHYSICS_TRIANGLE_MESH_SHAPE_AUTO_PTR_HPP_INCLUDED

#include "shape_fwd.hpp"
#include <memory>

namespace insula
{
namespace physics
{
namespace triangle_mesh
{
typedef
std::auto_ptr<shape>
shape_auto_ptr;
}
}
}

#endif
