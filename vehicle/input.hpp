#ifndef INSULA_VEHICLE_INPUT_HPP_INCLUDED
#define INSULA_VEHICLE_INPUT_HPP_INCLUDED

#include "../physics/vehicle/object_fwd.hpp"
#include "../input_delegator_fwd.hpp"
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
		input_delegator &,
		physics::vehicle::object &);

	void
	is_active(
		bool);

	bool 
	is_active() const;
private:
	physics::vehicle::object &vehicle_;
	bool is_active_;

	fcppt::signal::scoped_connection input_connection_;

	typedef
	std::map
	<
		char,
		bool
	>
	key_map;

	key_map key_pressed_;
	
	void
	callback(
		sge::input::key_pair const &);
};
}
}

#endif
