#ifndef INSULA_GHOST_INSTANCE_HPP_INCLUDED
#define INSULA_GHOST_INSTANCE_HPP_INCLUDED

#include "../model/instance.hpp"
#include "../graphics/scalar.hpp"
#include "../graphics/vec3.hpp"
#include "../physics/broadphase/manager.hpp"
#include "../physics/ghost.hpp"

namespace insula
{
namespace ghost
{
class instance
:
	public model::instance
{
public:
	explicit
	instance(
		graphics::mat4 const &,
		physics::broadphase::manager &,
		physics::ghost_parameters const &);

	bool
	is_visible() const;

	graphics::scalar
	distance_to(
		graphics::vec3 const &) const;
private:
	physics::broadphase::manager &broadphase_;
	physics::ghost physics_;
};
}
}

#endif
