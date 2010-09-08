#include "static_model_instance.hpp"
#include "model_backend.hpp"
#include "graphics/shader/object.hpp"
#include "graphics/camera/object.hpp"
#include "model/object.hpp"
#include "graphics/mat4.hpp"
#include "graphics/vec3.hpp"
#include "physics/world.hpp"
#include <fcppt/math/matrix/structure_cast.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/vector/structure_cast.hpp>

insula::static_model_instance::static_model_instance(
	physics::world &_physics_world,
	physics::vec3 const &_position,
	physics::shared_shape_ptr _shape,
	physics::solidity::type const _solidity)
:
	model_instance(
		fcppt::math::matrix::translation(
			fcppt::math::vector::structure_cast<graphics::vec3>(
				_position))),
	physics_world_(
		_physics_world),
	physics_(
		_physics_world,
		_position,
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
