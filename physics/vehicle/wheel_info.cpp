#include "wheel_info.hpp"
#include "../scalar.hpp"

insula::physics::vehicle::wheel_info::wheel_info()
:
	position_(
		vec3::null()),
	is_front_wheel_(
		false),
	// Taken from the standard btVehicleTuning constructor
	suspension_stiffness_(
		static_cast<insula::physics::scalar>(5.88)),
	// Taken from the standard btVehicleTuning constructor
	suspension_compression_(
		static_cast<insula::physics::scalar>(0.83)),
	// Taken from the standard btVehicleTuning constructor
	suspension_damping_(
		static_cast<insula::physics::scalar>(0.88)),
	// Taken from the standard btVehicleTuning constructor
	suspension_travel_cm_(
		static_cast<insula::physics::scalar>(500)),
	// Taken from VehicleDemo
	suspension_rest_length_(
		static_cast<insula::physics::scalar>(0.6)),
	// Taken from VehicleDemo
	damping_relaxation_(
		static_cast<insula::physics::scalar>(2.3)),
	// Taken from VehicleDemo
	damping_compression_(
		static_cast<insula::physics::scalar>(4.4)), 
	// Taken from the standard btVehicleTuning constructor
	friction_slip_(
		static_cast<insula::physics::scalar>(10.5)),
	// Taken from VehicleDemo
	roll_influence_(
		static_cast<insula::physics::scalar>(0.1)),
	// Taken from the standard btVehicleTuning constructor
	maximum_suspension_force_(
		static_cast<insula::physics::scalar>(6000)),
	gets_engine_force_(
		false),
	gets_breaking_force_(
		false),
	gets_steering_(
		false)
{
}

insula::physics::vehicle::wheel_info &
insula::physics::vehicle::wheel_info::position(
	vec3 const &p) 
{ 
	position_ = p; 
	return *this; 
}

insula::physics::vec3 const &
insula::physics::vehicle::wheel_info::position() const 
{ 
	return position_; 
}

insula::physics::vehicle::wheel_info &
insula::physics::vehicle::wheel_info::is_front_wheel(
	bool const i) 
{ 
	is_front_wheel_ = i; 
	return *this; 
}

bool 
insula::physics::vehicle::wheel_info::is_front_wheel() const 
{ 
	return is_front_wheel_; 
}

insula::physics::vehicle::wheel_info &
insula::physics::vehicle::wheel_info::suspension_stiffness(
	insula::physics::scalar const s) 
{ 
	suspension_stiffness_ = s; 
	return *this;
}

insula::physics::scalar 
insula::physics::vehicle::wheel_info::suspension_stiffness() const 
{ 
	return suspension_stiffness_; 
}

insula::physics::vehicle::wheel_info &
insula::physics::vehicle::wheel_info::suspension_compression(
	scalar const s) 
{ 
	suspension_compression_ = s; 
	return *this; 
}

insula::physics::scalar 
insula::physics::vehicle::wheel_info::suspension_compression() const 
{ 
	return suspension_compression_; 
}

insula::physics::vehicle::wheel_info &
insula::physics::vehicle::wheel_info::suspension_damping(
	scalar const s) 
{ 
	suspension_damping_ = s; 
	return *this; 
}

insula::physics::scalar 
insula::physics::vehicle::wheel_info::suspension_damping() const 
{ 
	return suspension_damping_; 
}

insula::physics::vehicle::wheel_info &
insula::physics::vehicle::wheel_info::suspension_travel_cm(
	insula::physics::scalar const s) 
{ 
	suspension_travel_cm_ = s; 
	return *this; 
}

insula::physics::scalar 
insula::physics::vehicle::wheel_info::suspension_travel_cm() const 
{ 
	return suspension_travel_cm_; 
}

insula::physics::vehicle::wheel_info &
insula::physics::vehicle::wheel_info::suspension_rest_length(
	scalar const s) 
{ 
	suspension_rest_length_ = s; 
	return *this; 
}

insula::physics::scalar 
insula::physics::vehicle::wheel_info::suspension_rest_length() const 
{ 
	return suspension_rest_length_; 
}

insula::physics::vehicle::wheel_info &
insula::physics::vehicle::wheel_info::damping_relaxation(
	insula::physics::scalar const s) 
{ 
	damping_relaxation_ = s; 
	return *this; 
}

insula::physics::scalar 
insula::physics::vehicle::wheel_info::damping_relaxation() const 
{ 
	return damping_relaxation_; 
}

insula::physics::vehicle::wheel_info &
insula::physics::vehicle::wheel_info::damping_compression(
	scalar const s) 
{ 
	damping_compression_ = s; 
	return *this; 
}

insula::physics::scalar 
insula::physics::vehicle::wheel_info::damping_compression() const 
{ 
	return damping_compression_; 
}

insula::physics::vehicle::wheel_info &
insula::physics::vehicle::wheel_info::friction_slip(
	scalar const s) 
{ 
	friction_slip_ = s; 
	return *this; 
}

insula::physics::scalar 
insula::physics::vehicle::wheel_info::friction_slip() const 
{ 
	return friction_slip_; 
}

insula::physics::vehicle::wheel_info &
insula::physics::vehicle::wheel_info::roll_influence(
	scalar const s) 
{ 
	roll_influence_ = s; 
	return *this; 
}

insula::physics::scalar 
insula::physics::vehicle::wheel_info::roll_influence() const 
{ 
	return roll_influence_; 
}

insula::physics::vehicle::wheel_info &
insula::physics::vehicle::wheel_info::maximum_suspension_force(
	scalar const s) 
{ 
	maximum_suspension_force_ = s; 
	return *this; 
}

insula::physics::scalar 
insula::physics::vehicle::wheel_info::maximum_suspension_force() const 
{ 
	return maximum_suspension_force_; 
}

insula::physics::vehicle::wheel_info &
insula::physics::vehicle::wheel_info::gets_engine_force(
	bool const i) 
{ 
	gets_engine_force_ = i; 
	return *this; 
}

bool 
insula::physics::vehicle::wheel_info::gets_engine_force() const 
{ 
	return gets_engine_force_; 
}

insula::physics::vehicle::wheel_info &
insula::physics::vehicle::wheel_info::gets_breaking_force(
	bool const i) 
{ 
	gets_breaking_force_ = i; 
	return *this; 
}

bool 
insula::physics::vehicle::wheel_info::gets_breaking_force() const 
{ 
	return gets_breaking_force_; 
}

insula::physics::vehicle::wheel_info &
insula::physics::vehicle::wheel_info::gets_steering(
	bool const i) 
{ 
	gets_steering_ = i; 
	return *this; 
}

bool 
insula::physics::vehicle::wheel_info::gets_steering() const 
{ 
	return gets_steering_; 
}
