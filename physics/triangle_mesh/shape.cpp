#include "shape.hpp"
#include <fcppt/io/cout.hpp>
#include <BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h>

insula::physics::triangle_mesh::shape::shape(
	vertex_array const &_vertices,
	index_array const &_indices)
:
	vertices_(
		_vertices),
	indices_(
		_indices),
	array_(
		static_cast<int>(
			indices_.size() / 3),
		indices_.data(),
		static_cast<int>(
			3*sizeof(int)),
		static_cast<int>(
			vertices_.size()),
		&vertices_.front()[0],
		static_cast<int>(
			sizeof(btVector3))),
	impl_(
		&array_,
		// use quantized aabb compression, wtf?
		true)
{
}

btBvhTriangleMeshShape &
insula::physics::triangle_mesh::shape::impl()
{
	return impl_;
}

insula::physics::shared_shape_ptr const 
insula::physics::triangle_mesh::shape::spawn_scaled_instance(
	scalar const scaling)
{
	return 
		shared_shape_ptr(
			new btScaledBvhTriangleMeshShape(
				&impl_,
				btVector3(
					scaling,
					scaling,
					scaling)));
}
