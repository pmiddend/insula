#ifndef INSULA_PHYSICS_SPHERE_HPP_INCLUDED
#define INSULA_PHYSICS_SPHERE_HPP_INCLUDED

#include "world_fwd.hpp"
#include "vec3.hpp"
#include "scalar.hpp"
#include "../graphics/mat4.hpp"
#include "../graphics/camera/object_fwd.hpp"
#include "../graphics/shader.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/index_buffer_ptr.hpp>
#include <LinearMath/btMotionState.h>
#include <LinearMath/btTransform.h>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <memory>

class btSphereShape;
class btRigidBody;
class btMotionState;

namespace insula
{
namespace physics
{
class sphere
:
	public btMotionState
{
public:
	sphere(sphere const &) = delete;
	sphere &operator=(sphere const &) = delete;

	explicit
	sphere(
		sge::renderer::device_ptr,
		graphics::camera::object &,
		world &,
		scalar radius,
		scalar mass,
		vec3 const &position);

	void
	render();
	
	~sphere();
private:
	world &world_;
	graphics::camera::object &camera_;
	btTransform transform_;
	std::unique_ptr<btSphereShape> shape_;
	std::unique_ptr<btMotionState> motion_state_;
	std::unique_ptr<btRigidBody> body_;
	sge::renderer::device_ptr const renderer_;
	sge::renderer::vertex_buffer_ptr vb_;
	sge::renderer::index_buffer_ptr ib_;
	graphics::shader_old shader_;
	graphics::mat4 mat_transformed_;

	// @override
	void
	getWorldTransform(
		btTransform &) const;

	// @override
	void
	setWorldTransform(
		btTransform const &);
};
}
}

#endif
