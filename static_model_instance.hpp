#ifndef INSULA_STATIC_MODEL_INSTANCE_HPP_INCLUDED
#define INSULA_STATIC_MODEL_INSTANCE_HPP_INCLUDED

#include "model/instance.hpp"
#include "scene/transparent_instance.hpp"
#include "scene/backend_ptr.hpp"
#include "graphics/scalar.hpp"
#include "graphics/vec3.hpp"
#include "physics/world_fwd.hpp"
#include "physics/static_model.hpp"
#include "physics/vec3.hpp"
#include "physics/mat3.hpp"
#include "physics/solidity.hpp"
#include "physics/shared_shape_ptr.hpp"

namespace insula
{
class static_model_instance
:
	public model::instance
{
public:
	explicit
	static_model_instance(
		graphics::mat4 const &,
		physics::world &,
		physics::vec3 const&,
		physics::mat3 const&,
		physics::shared_shape_ptr,
		physics::solidity::type);

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
	physics::static_model physics_;
};
}

#endif
