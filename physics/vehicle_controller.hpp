#ifndef INSULA_PHYSICS_VEHICLE_CONTROLLER_HPP_INCLUDED
#define INSULA_PHYSICS_VEHICLE_CONTROLLER_HPP_INCLUDED

#include "vehicle_fwd.hpp"
#include "../input_delegator_fwd.hpp"
#include <sge/input/key_pair_fwd.hpp>
#include <fcppt/signal/scoped_connection.hpp>

namespace insula
{
namespace physics
{
class vehicle_controller
{
public:
	vehicle_controller(vehicle_controller const &) = delete;
	vehicle_controller &operator=(vehicle_controller const &) = delete;

	explicit
	vehicle_controller(
		input_delegator &,
		vehicle &);

	void
	is_active(
		bool);

	bool 
	is_active() const;
private:
	vehicle &vehicle_;
	bool is_active_;

	fcppt::signal::scoped_connection input_connection_;
	
	void
	callback(
		sge::input::key_pair const &);
};
}
}

#endif
