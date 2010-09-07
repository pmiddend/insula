#ifndef INSULA_STATIC_MODEL_INSTANCE_HPP_INCLUDED
#define INSULA_STATIC_MODEL_INSTANCE_HPP_INCLUDED

#include "scene/transparent_instance.hpp"
#include "scene/backend_ptr.hpp"
#include "graphics/scalar.hpp"
#include "graphics/vec3.hpp"
#include "physics/world_fwd.hpp"
#include "physics/static_model.hpp"
#include "physics/vec3.hpp"
#include "physics/solidity.hpp"
#include "physics/shape_ptr.hpp"

namespace insula
{
class static_model_instance
:
	public scene::transparent_instance
{
public:
	explicit
	static_model_instance(
		physics::world &,
		physics::vec3 const&,
		physics::shape_ptr,
		physics::solidity::type);

	void
	render(
		scene::backend &);

	graphics::scalar
	distance_to(
		graphics::vec3 const &) const { return 0; }

	physics::static_model const &
	physics_model() const;
private:
	physics::world &physics_world_;
	physics::static_model physics_;
};
}

#endif
