#ifndef INSULA_PHYSICS_RIGID_OBJECT_HPP_INCLUDED
#define INSULA_PHYSICS_RIGID_OBJECT_HPP_INCLUDED

#include "../object.hpp"
#include "solidity.hpp"
#include "../vec3.hpp"
#include "../mat4.hpp"
#include "../world_fwd.hpp"
#include "parameters_fwd.hpp"
#include "../motion_state_fwd.hpp"
#include "../scoped_body.hpp"
#include "../shared_shape_ptr.hpp"
#include <memory>

class btRigidBody;
class btMotionState;
class btCollisionShape;

namespace insula
{
namespace physics
{
namespace rigid
{
class object
:
	public physics::object
{
public:
	explicit
	object(
		parameters const &);

	vec3 const 
	position() const;

	mat4 const
	world_transform() const;

	~object();
private:
	std::unique_ptr<motion_state> motion_state_;
	shared_shape_ptr shape_;
	std::unique_ptr<btRigidBody> body_;
	std::unique_ptr<scoped_body> world_scope_;
};
}
}
}

#endif
