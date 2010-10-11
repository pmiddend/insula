#include "rigid_model.hpp"
#include "graphics/mat4.hpp"
#include "model/backend.hpp"
#include "model/object.hpp"
#include "physics/broadphase/manager.hpp"
#include "physics/rigid/parameters.hpp"
#include "graphics/shader/object.hpp"
#include "graphics/camera/object.hpp"
#include "physics/bullet_to_vec3.hpp"
#include <fcppt/math/matrix/structure_cast.hpp>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <algorithm>

insula::rigid_model::rigid_model(
	graphics::mat4 const &_physics_to_model,
	physics::broadphase::manager &_broadphase,
	physics::rigid::parameters const &params)
:
	physics_to_model_(
		_physics_to_model),
	shape_(
		params.shape),
	broadphase_(
		_broadphase),
	physics_(
		params)
{
}

bool
insula::rigid_model::is_visible() const
{
	return physics_.last_seen() == broadphase_.current_iteration();
}

insula::physics::rigid::object const &
insula::rigid_model::physics_model() const
{
	return physics_;
}

void
insula::rigid_model::render(
	scene::backend &back)
{
	model::backend &realback = 
		dynamic_cast<model::backend &>(
			back);

	realback.modelview(
		realback.camera().world() 
			* fcppt::math::matrix::structure_cast<graphics::mat4>(
				physics_model().world_transform())
			* physics_to_model_);

	realback.model().render();
}

insula::graphics::scalar
insula::rigid_model::distance_to(
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
