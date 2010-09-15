#ifndef INSULA_PROP_INSTANCE_PARAMETERS_HPP_INCLUDED
#define INSULA_PROP_INSTANCE_PARAMETERS_HPP_INCLUDED

#include "../physics/world_fwd.hpp"
#include "manager_fwd.hpp"

namespace insula
{
namespace prop
{
class instance_parameters
{
public:
	manager &manager_;
	physics::world &world;

	explicit
	instance_parameters(
		physics::world &world,
		manager &manager_)
	:
		world(world),
		manager_(manager_)
	{
	}
};
}
}

#endif
