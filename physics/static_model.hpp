#ifndef INSULA_PHYSICS_STATIC_MODEL_HPP_INCLUDED
#define INSULA_PHYSICS_STATIC_MODEL_HPP_INCLUDED

#include "object.hpp"
#include "solidity.hpp"
#include "vec3.hpp"
#include "model_approximation.hpp"
#include "world_fwd.hpp"
#include "motion_state_fwd.hpp"
#include "scoped_body.hpp"
#include "../model/object_fwd.hpp"
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
		model::object &,
		model_approximation const &,
		solidity::type);

	void
	render();

	~static_model();
private:
	model::object &model_;
	std::unique_ptr<btRigidBody> body_;
	std::unique_ptr<motion_state> motion_state_;
	std::unique_ptr<btCollisionShape> shape_;
	scoped_body world_scope_;
};
}
}

#endif
