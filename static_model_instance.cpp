#include "static_model_instance.hpp"
#include "model_backend.hpp"
#include "graphics/shader/object.hpp"
#include "graphics/camera/object.hpp"
#include "graphics/mat4.hpp"
#include "physics/world.hpp"
#include <fcppt/math/matrix/structure_cast.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>

insula::static_model_instance::static_model_instance(
	physics::world &_physics_world,
	physics::vec3 const &_position,
	physics::shape_ptr _shape,
	physics::solidity::type const _solidity)
:
	physics_world_(
		_physics_world),
	physics_(
		_physics_world,
		_position,
		_shape,
		_solidity)
{
}

void
insula::static_model_instance::render(
	scene::backend &back)
{
	if (physics_.last_seen() != physics_world_.current_iteration())
		return;

	model_backend &realback = 
		dynamic_cast<model_backend &>(
			back);

	realback.shader().set_uniform(
		"mvp",
		realback.camera().mvp() * 
		fcppt::math::matrix::structure_cast<graphics::mat4>(
			physics_.world_transform()));
}
