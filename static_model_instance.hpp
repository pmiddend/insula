#ifndef INSULA_STATIC_MODEL_INSTANCE_HPP_INCLUDED
#define INSULA_STATIC_MODEL_INSTANCE_HPP_INCLUDED

#include "model/instance.hpp"
#include "scene/transparent_instance.hpp"
#include "scene/backend_ptr.hpp"
#include "graphics/scalar.hpp"
#include "graphics/vec3.hpp"
#include "physics/broadphase/manager_fwd.hpp"
#include "physics/static_model.hpp"
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
		physics::broadphase::manager &,
		physics::static_model_parameters const &);

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
	physics::broadphase::manager &broadphase_;
	physics::static_model physics_;
};
}

#endif
