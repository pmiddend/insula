#ifndef INSULA_VEHICLE_INPUT_HPP_INCLUDED
#define INSULA_VEHICLE_INPUT_HPP_INCLUDED

#include "parameters_fwd.hpp"
#include "../physics/vehicle/object_fwd.hpp"
#include "../physics/scalar.hpp"
#include "../time_delta.hpp"
#include <sge/input/key_pair_fwd.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <map>

namespace insula
{
namespace vehicle
{
class input
{
public:
	input(input const &) = delete;
	input &operator=(input const &) = delete;

	explicit
	input(
		physics::vehicle::object &,
		parameters const &);

	void
	is_active(
		bool);

	bool 
	is_active() const;

	void
	update(
		time_delta);
private:
	enum action_type
	{
		action_accelerate,
		action_reverse,
		action_brake,
		action_left,
		action_right
	};

	typedef
	std::map
	<
		action_type,
		bool
	>
	key_map;

	physics::vehicle::object &vehicle_;
	bool is_active_;
	fcppt::signal::scoped_connection input_connection_;
	physics::scalar const steering_speed_,braking_speed_,engine_speed_;
	physics::scalar steering_,brake_,engine_;
	key_map key_pressed_;
	
	void
	callback(
		sge::input::key_pair const &);
};
}
}

#endif
