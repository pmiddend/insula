#include "ghost_instance.hpp"
#include "physics/ghost_parameters.hpp"
#include "physics/world.hpp"
#include "physics/bullet_to_vec3.hpp"
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/length.hpp>
#include <algorithm>

insula::ghost_instance::ghost_instance(
	graphics::mat4 const &_model_trafo,
	physics::ghost_parameters const &params)
:
	model::instance(
		_model_trafo),
	shape_(
		params.shape),
	physics_world_(
		params.world_),
	physics_(
		params)
{
}

bool
insula::ghost_instance::is_visible() const
{
	return physics_.last_seen() == physics_world_.current_iteration();
}

insula::graphics::scalar
insula::ghost_instance::distance_to(
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
