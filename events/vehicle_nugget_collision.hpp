#ifndef INSULA_EVENTS_VEHICLE_NUGGET_COLLISION_HPP_INCLUDED
#define INSULA_EVENTS_VEHICLE_NUGGET_COLLISION_HPP_INCLUDED

#include "../physics/vehicle/object_fwd.hpp"
#include "../physics/static_model_fwd.hpp"
#include <boost/statechart/event.hpp>

namespace insula
{
namespace events
{
class vehicle_nugget_collision
:
	public boost::statechart::event<vehicle_nugget_collision>
{
public:
	explicit
	vehicle_nugget_collision(
		physics::vehicle::object &,
		physics::static_model &);
	
	physics::vehicle::object &
	vehicle() const;

	physics::static_model &
	nugget() const;
private:
	physics::vehicle::object &vehicle_;
	physics::static_model &nugget_;
};
}
}

#endif
