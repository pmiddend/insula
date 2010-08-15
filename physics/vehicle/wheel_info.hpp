#ifndef INSULA_PHYSICS_VEHICLE_WHEEL_INFO_HPP_INCLUDED
#define INSULA_PHYSICS_VEHICLE_WHEEL_INFO_HPP_INCLUDED

#include "../vec3.hpp"
#include <fcppt/math/vector/basic_impl.hpp>

namespace insula
{
namespace physics
{
namespace vehicle
{
class wheel_info
{
public:
	explicit
	wheel_info();

	wheel_info &
	position(
		vec3 const &);

	vec3 const &
	position() const;

	wheel_info &
	is_front_wheel(
		bool);
	bool 
	is_front_wheel() const;

	wheel_info &
	suspension_stiffness(
		scalar);
	scalar 
	suspension_stiffness() const;

	wheel_info &
	suspension_compression(
		scalar);
	scalar 
	suspension_compression() const;

	wheel_info &
	suspension_damping(
		scalar);
	scalar 
	suspension_damping() const;

	wheel_info &
	suspension_travel_cm(
		scalar);
	scalar 
	suspension_travel_cm() const;

	wheel_info &
	suspension_rest_length(
		scalar);
	scalar 
	suspension_rest_length() const;

	wheel_info &
	damping_relaxation(
		scalar);
	scalar 
	damping_relaxation() const;

	wheel_info &
	damping_compression(
		scalar);
	scalar 
	damping_compression() const;

	wheel_info &
	friction_slip(
		scalar);
	scalar 
	friction_slip() const;

	wheel_info &
	roll_influence(
		scalar);
	scalar 
	roll_influence() const;

	wheel_info &
	maximum_suspension_force(
		scalar);
	scalar 
	maximum_suspension_force() const;

	wheel_info &
	gets_engine_force(
		bool);
	bool 
	gets_engine_force() const;

	wheel_info &
	gets_breaking_force(
		bool);
	bool 
	gets_breaking_force() const;

	wheel_info &
	gets_steering(
		bool);
	bool 
	gets_steering() const;
private:
	vec3 position_;
	bool is_front_wheel_;
	scalar suspension_stiffness_;
	scalar suspension_compression_;
	scalar suspension_damping_;
	scalar suspension_travel_cm_;
	scalar suspension_rest_length_;
	scalar damping_relaxation_;
	scalar damping_compression_;
	scalar friction_slip_;
	scalar roll_influence_;
	scalar maximum_suspension_force_;
	bool gets_engine_force_;
	bool gets_breaking_force_;
	bool gets_steering_;
};
}
}
}

#endif
