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
/**
	This class connects a physics::rigid::object to a model::object. The
	physics object can have an offset relative to the model. However,
	both have to be synchronized because the rigid object might be
	moving and/or rotating. Let me elaborate: Each vertex 'v' of the
	model can be transformed in the following way:

	v_{new} = translation(origin) * rotation(angle,axis) * scaling(factor) * v

	The physics object's points 'v' (or center of mass), are transformed in
	the following way:

	v_{new} = translation(origin) * rotation(angle,axis) * translation(offset) * v

	The physics world regularly updates origin, angle and axis and gives
	us a new position and rotation matrix. To calculate the new _model_
	transformation, we supply a "physics to model" transition matrix,
	which consists of a backtranslation by -offset and a scaling matrix.
 */
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
