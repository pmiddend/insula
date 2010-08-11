#ifndef INSULA_PHYSICS_WHEEL_INFO_HPP_INCLUDED
#define INSULA_PHYSICS_WHEEL_INFO_HPP_INCLUDED

#include "vec3.hpp"
#include <fcppt/math/vector/basic_impl.hpp>

namespace insula
{
namespace physics
{
class wheel_info
{
public:
	explicit
	wheel_info()
	:
		position_(
			vec3::null()),
		is_front_wheel_(
			false),
		// Taken from the standard btVehicleTuning constructor
		suspension_stiffness_(
			static_cast<scalar>(5.88)),
		// Taken from the standard btVehicleTuning constructor
		suspension_compression_(
			static_cast<scalar>(0.83)),
		// Taken from the standard btVehicleTuning constructor
		suspension_damping_(
			static_cast<scalar>(0.88)),
		// Taken from the standard btVehicleTuning constructor
		suspension_travel_cm_(
			static_cast<scalar>(500)),
		// Taken from VehicleDemo
		suspension_rest_length_(
			static_cast<scalar>(0.6)),
		// Taken from VehicleDemo
		damping_relaxation_(
			static_cast<scalar>(2.3)),
		// Taken from VehicleDemo
		damping_compression_(
			static_cast<scalar>(4.4)), 
		// Taken from the standard btVehicleTuning constructor
		friction_slip_(
			static_cast<scalar>(10.5)),
		// Taken from VehicleDemo
		roll_influence_(
			static_cast<scalar>(0.1)),
		// Taken from the standard btVehicleTuning constructor
		maximum_suspension_force_(
			static_cast<scalar>(6000)),
		gets_engine_force_(
			false),
		gets_breaking_force_(
			false),
		gets_steering_(
			false)
	{
	}

	wheel_info &position(vec3 const &p) { position_ = p; return *this; }
	vec3 const &position() const { return position_; }

	wheel_info &is_front_wheel(bool const i) { is_front_wheel_ = i; return *this; }
	bool is_front_wheel() const { return is_front_wheel_; }

	wheel_info &suspension_stiffness(scalar const s) { suspension_stiffness_ = s; return *this; }
	scalar suspension_stiffness() const { return suspension_stiffness_; }

	wheel_info &suspension_compression(scalar const s) { suspension_compression_ = s; return *this; }
	scalar suspension_compression() const { return suspension_compression_; }

	wheel_info &suspension_damping(scalar const s) { suspension_damping_ = s; return *this; }
	scalar suspension_damping() const { return suspension_damping_; }

	wheel_info &suspension_travel_cm(scalar const s) { suspension_travel_cm_ = s; return *this; }
	scalar suspension_travel_cm() const { return suspension_travel_cm_; }

	wheel_info &suspension_rest_length(scalar const s) { suspension_rest_length_ = s; return *this; }
	scalar suspension_rest_length() const { return suspension_rest_length_; }

	wheel_info &damping_relaxation(scalar const s) { damping_relaxation_ = s; return *this; }
	scalar damping_relaxation() const { return damping_relaxation_; }

	wheel_info &damping_compression(scalar const s) { damping_compression_ = s; return *this; }
	scalar damping_compression() const { return damping_compression_; }

	wheel_info &friction_slip(scalar const s) { friction_slip_ = s; return *this; }
	scalar friction_slip() const { return friction_slip_; }

	wheel_info &roll_influence(scalar const s) { roll_influence_ = s; return *this; }
	scalar roll_influence() const { return roll_influence_; }

	wheel_info &maximum_suspension_force(scalar const s) { maximum_suspension_force_ = s; return *this; }
	scalar maximum_suspension_force() const { return maximum_suspension_force_; }

	wheel_info &gets_engine_force(bool const i) { gets_engine_force_ = i; return *this; }
	bool gets_engine_force() const { return gets_engine_force_; }

	wheel_info &gets_breaking_force(bool const i) { gets_breaking_force_ = i; return *this; }
	bool gets_breaking_force() const { return gets_breaking_force_; }

	wheel_info &gets_steering(bool const i) { gets_steering_ = i; return *this; }
	bool gets_steering() const { return gets_steering_; }
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

#endif
