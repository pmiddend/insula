#include "ghost_instance.hpp"
#include "physics/ghost_parameters.hpp"
#include "physics/bullet_to_vec3.hpp"
#include "physics/broadphase/manager.hpp"
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/length.hpp>
#include <algorithm>

insula::ghost_instance::ghost_instance(
	graphics::mat4 const &_model_trafo,
	physics::broadphase::manager &_broadphase,
	physics::ghost_parameters const &params)
:
	model::instance(
		_model_trafo),
	broadphase_(
		_broadphase),
	physics_(
		params)
{
}

bool
insula::ghost_instance::is_visible() const
{
	return physics_.last_seen() == broadphase_.current_iteration();
}

insula::graphics::scalar
insula::ghost_instance::distance_to(
	graphics::vec3 const &) const
{
	return static_cast<graphics::scalar>(0);
}
