#ifndef INSULA_PHYSICS_VEHICLE_PARAMETERS_HPP_INCLUDED
#define INSULA_PHYSICS_VEHICLE_PARAMETERS_HPP_INCLUDED

#include "../world_fwd.hpp"
#include "../scalar.hpp"
#include "../shared_shape_ptr.hpp"
#include "../vec3.hpp"
#include "../box.hpp"
#include "wheel_info_sequence.hpp"
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/box/basic_impl.hpp>

namespace insula
{
namespace physics
{
namespace vehicle
{
class parameters
{
public:
	world &world_;
	shared_shape_ptr chassis_shape;
	scalar mass;
	scalar chassis_position;
	scalar steering_clamp;
	vec3 const &position;
	scalar max_engine_force;
	scalar max_breaking_force;
	scalar max_speed;
	scalar track_connection;
	box const &wheel_bb;
	wheel_info_sequence const &wheel_infos;

	explicit
	parameters(
		world &world_,
		shared_shape_ptr chassis_shape,
		scalar mass,
		scalar chassis_position,
		scalar steering_clamp,
		vec3 const &position,
		scalar max_engine_force,
		scalar max_breaking_force,
		scalar max_speed,
		scalar track_connection,
		box const &wheel_bb,
		wheel_info_sequence const &wheel_infos)
	:
		world_(world_),
		chassis_shape(chassis_shape),
		mass(mass),
		chassis_position(chassis_position),
		steering_clamp(steering_clamp),
		position(position),
		max_engine_force(max_engine_force),
		max_breaking_force(max_breaking_force),
		max_speed(max_speed),
		track_connection(track_connection),
		wheel_bb(wheel_bb),
		wheel_infos(wheel_infos)
	{
	}
};
}
}
}

#endif
