#ifndef INSULA_RIGID_MODEL_HPP_INCLUDED
#define INSULA_RIGID_MODEL_HPP_INCLUDED

#include "graphics/mat4.hpp"
#include "graphics/vec3.hpp"
#include "graphics/scalar.hpp"
#include "physics/broadphase/manager_fwd.hpp"
#include "physics/rigid/object.hpp"
#include "physics/rigid/parameters_fwd.hpp"
#include "physics/shared_shape_ptr.hpp"
#include "scene/backend_fwd.hpp"
#include "scene/transparent_instance.hpp"
#include <fcppt/math/matrix/basic_impl.hpp>

namespace insula
{
class rigid_model
:
	public scene::transparent_instance
{
public:
	explicit
	rigid_model(
		graphics::mat4 const &physics_to_model,
		physics::broadphase::manager &,
		physics::rigid::parameters const &);

	bool
	is_visible() const;

	// We need this in the collision callbacks
	physics::rigid::object const &
	physics_model() const;

	void
	render(
		scene::backend &);

	graphics::scalar
	distance_to(
		graphics::vec3 const &) const;
private:
	graphics::mat4 physics_to_model_;
	physics::shared_shape_ptr shape_;
	// For the visibility test
	physics::broadphase::manager &broadphase_;
	physics::rigid::object physics_;
};
}

#endif
