#include "vehicle_nugget_collision.hpp"

insula::events::vehicle_nugget_collision::vehicle_nugget_collision(
	physics::vehicle::object &_vehicle,
	physics::static_model &_nugget)
:
	vehicle_(
		_vehicle),
	nugget_(
		_nugget)
{
}

insula::physics::vehicle::object &
insula::events::vehicle_nugget_collision::vehicle() const
{
	return vehicle_;
}

insula::physics::static_model &
insula::events::vehicle_nugget_collision::nugget() const
{
	return nugget_;
}
