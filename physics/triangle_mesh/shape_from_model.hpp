#ifndef INSULA_PHYSICS_TRIANGLE_MESH_SHAPE_FROM_MODEL_HPP_INCLUDED
#define INSULA_PHYSICS_TRIANGLE_MESH_SHAPE_FROM_MODEL_HPP_INCLUDED

#include "shape_auto_ptr.hpp"
#include <sge/model/object_ptr.hpp>

namespace insula
{
namespace physics
{
namespace triangle_mesh
{
shape_auto_ptr
shape_from_model(
	sge::model::object_ptr);
}
}
}

#endif
