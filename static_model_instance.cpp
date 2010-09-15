#include "static_model_instance.hpp"
#include "physics/broadphase/manager.hpp"
#include "physics/bullet_to_vec3.hpp"
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/length.hpp>
#include <algorithm>

insula::static_model_instance::static_model_instance(
	graphics::mat4 const &_model_trafo,
	physics::broadphase::manager &_broadphase,
	physics::static_model_parameters const &params)
:
	model::instance(
		_model_trafo),
	shape_(
		params.shape),
	broadphase_(
		_broadphase),
	physics_(
		params)
{
}

bool
insula::static_model_instance::is_visible() const
{
	return physics_.last_seen() == broadphase_.current_iteration();
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
