#ifndef INSULA_PHYSICS_STATIC_MODEL_HPP_INCLUDED
#define INSULA_PHYSICS_STATIC_MODEL_HPP_INCLUDED

#include "object.hpp"
#include "solidity.hpp"
#include "vec3.hpp"
#include "mat4.hpp"
#include "world_fwd.hpp"
#include "motion_state_fwd.hpp"
#include "scoped_body.hpp"
#include "shape_ptr.hpp"
#include <memory>

class btRigidBody;
class btMotionState;
class btCollisionShape;

namespace insula
{
namespace physics
{
class static_model
:
	public object
{
public:
	explicit
	static_model(
		world &,
		vec3 const &,
		shape_ptr,	
		solidity::type);

	mat4 const
	world_transform() const;

	~static_model();
private:
	std::unique_ptr<btRigidBody> body_;
	std::unique_ptr<motion_state> motion_state_;
	shape_ptr shape_;
	scoped_body world_scope_;
};
}
}

#endif
