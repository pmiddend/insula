#include "static_model_instance.hpp"
#include "physics/world.hpp"
#include "physics/bullet_to_vec3.hpp"
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/length.hpp>
#include <algorithm>

insula::static_model_instance::static_model_instance(
	graphics::mat4 const &_model_trafo,
	physics::world &_physics_world,
	physics::vec3 const &_position,
	physics::mat3 const &_transformation,
	physics::shared_shape_ptr _shape,
	physics::solidity::type const _solidity)
:
	model_instance(
		_model_trafo),
	shape_(
		_shape),
	physics_world_(
		_physics_world),
	physics_(
		_physics_world,
		_position,
		_transformation,
		_shape,
		_solidity)
{
}

bool
insula::static_model_instance::is_visible() const
{
	return physics_.last_seen() == physics_world_.current_iteration();
}

insula::physics::static_model const &
insula::static_model_instance::physics_model() const
{
	return physics_;
}

insula::graphics::scalar
insula::static_model_instance::distance_to(
	graphics::vec3 const &v) const
{
	btVector3 center;
	btScalar radius;
	shape_->getBoundingSphere(
		center,
		radius);
	return 
		std::max(
			static_cast<graphics::scalar>(0),
			length(
				physics::bullet_to_vec3(center) - v) 
			- radius);
}
