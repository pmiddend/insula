#include "static_model_instance.hpp"
#include "physics/world.hpp"

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
