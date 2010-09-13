#ifndef INSULA_GHOST_INSTANCE_HPP_INCLUDED
#define INSULA_GHOST_INSTANCE_HPP_

#include "model/instance.hpp"
#include "scene/transparent_instance.hpp"
#include "scene/backend_ptr.hpp"
#include "graphics/scalar.hpp"
#include "graphics/vec3.hpp"
#include "physics/world_fwd.hpp"
#include "physics/ghost.hpp"
#include "physics/shared_shape_ptr.hpp"

namespace insula
{
class ghost_instance
:
	public model::instance
{
public:
	explicit
	ghost_instance(
		graphics::mat4 const &,
		physics::ghost_parameters const &);

	bool
	is_visible() const;

	// We need this in the collision callbacks
	physics::static_model const &
	physics_model() const;

	graphics::scalar
	distance_to(
		graphics::vec3 const &) const;
private:
	physics::shared_shape_ptr shape_;
	physics::world &physics_world_;
	physics::ghost physics_;
};
}

#endif
