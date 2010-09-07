#ifndef INSULA_PHYSICS_VEHICLE_OBJECT_HPP_INCLUDED
#define INSULA_PHYSICS_VEHICLE_OBJECT_HPP_INCLUDED

#include "wheel_info_sequence.hpp"
#include "bullet_wrapper_fwd.hpp"
//#include "friction_constraint_fwd.hpp"
#include "../gizmo.hpp"
#include "../motion_state_fwd.hpp"
#include "../scoped_body.hpp"
#include "../scoped_action.hpp"
#include "../scoped_constraint.hpp"
#include "../../graphics/vec3.hpp"
#include "../upright_constraint.hpp"
#include "../vec3.hpp"
#include "../mat4.hpp"
#include "../box.hpp"
#include "../mat4_sequence.hpp"
#include "../world_fwd.hpp"
#include "../object.hpp"
#include "../shape_ptr.hpp"
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <memory>

class btRigidBody;
class btBoxShape;
class btCompoundShape;
class btCylinderShapeX;
class btTransform;
class btVehicleRaycaster;
class btRaycastVehicle;

namespace insula
{
namespace physics
{
namespace vehicle
{
class object
:
	public physics::object
{
public:
	object(object const &) = delete;
	object &operator=(object const &) = delete;
	
	explicit
	object(
		world &,
		shape_ptr chassis_shape,
		scalar mass,
		scalar chassis_position,
		scalar steering_clamp,
		vec3 const &position,
		scalar max_engine_force,
		scalar max_breaking_force,
		scalar max_speed,
		scalar track_connection,
		box const &wheel_bb,
 		wheel_info_sequence const &);

	void
	update();

	mat4 const
	chassis_transform() const;

	mat4_sequence const
	wheel_transforms() const;

	void 
	render();

	void
	engine_force(
		scalar);

	void
	breaking_force(
		scalar);

	void
	steering(
		scalar);

	scalar
	speed_kmh() const;

	// The camera needs this
	insula::physics::gizmo const
	gizmo() const;

	bool
	is_skidding() const;

	~object();
private:
	world &world_;
	shape_ptr chassis_shape_;
	std::unique_ptr<btCompoundShape> compound_;
	std::unique_ptr<btRigidBody> car_body_;
	std::unique_ptr<btCylinderShapeX> wheel_shape_;
	std::unique_ptr<btVehicleRaycaster> raycaster_; 
	std::unique_ptr<bullet_wrapper> vehicle_;
	std::unique_ptr<motion_state> motion_state_;
	wheel_info_sequence const wheels_;
	scalar max_engine_force_;
	scalar max_breaking_force_;
	scalar max_speed_;
	scalar current_engine_force_;
	scalar current_breaking_force_;
	scalar current_steering_;
	scalar steering_clamp_;
	std::unique_ptr<upright_constraint> constraint_;
	std::unique_ptr<scoped_constraint> scoped_constraint_;

//	std::unique_ptr<friction_constraint> friction_constraint_;
//	std::unique_ptr<scoped_constraint> scoped_friction_constraint_;

	bool is_skidding_;
	std::unique_ptr<scoped_body> world_body_scope_;
	scoped_action world_vehicle_scope_;
};
}
}
}

#endif
