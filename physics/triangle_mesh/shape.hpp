#ifndef INSULA_PHYSICS_TRIANGLE_MESH_SHAPE_HPP_INCLUDED
#define INSULA_PHYSICS_TRIANGLE_MESH_SHAPE_HPP_INCLUDED

#include "index_array.hpp"
#include "vertex_array.hpp"
#include "../shared_shape_ptr.hpp"
#include "../scalar.hpp"
#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h>

namespace insula
{
namespace physics
{
namespace triangle_mesh
{
class shape
{
public:
	shape(shape const &) = delete;
	shape &operator=(shape const &) = delete;

	explicit
	shape(
		vertex_array const &,
		index_array const &);

	btBvhTriangleMeshShape &
	impl();

	// This cannot be const since spawning a child shape changes the
	// parent shape (yeah, I know, wtf...)
	shared_shape_ptr const 
	spawn_scaled_instance(
		scalar scaling);
private:
	vertex_array vertices_;
	index_array indices_;
	btTriangleIndexVertexArray array_;
	btBvhTriangleMeshShape impl_;
};
}
}
}

#endif
