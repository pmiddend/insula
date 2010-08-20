#ifndef INSULA_PHYSICS_VEHICLE_OBJECT_HPP_INCLUDED
#define INSULA_PHYSICS_VEHICLE_OBJECT_HPP_INCLUDED

#include "../../model/object_ptr.hpp"
#include "../gizmo.hpp"
#include "../scoped_body.hpp"
#include "../scoped_action.hpp"
#include "../../graphics/mat4.hpp"
#include "../../graphics/vec3.hpp"
#include "../upright_constraint.hpp"
#include "wheel_info_sequence.hpp"
#include "../vec3.hpp"
#include "../world_fwd.hpp"
#include "../object.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <LinearMath/btMotionState.h>
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
	public btMotionState,
	public physics::object
{
public:
	object(object const &) = delete;
	object &operator=(object const &) = delete;
	
	explicit
	object(
		world &,
		sge::renderer::device_ptr,
		model::object_ptr chassis,
		scalar mass,
		scalar chassis_position,
		scalar steering_clamp,
		vec3 const &position,
		scalar max_engine_force,
		scalar max_breaking_force,
		scalar max_speed,
		model::object_ptr wheel,
 		wheel_info_sequence const &);

	void
	update();

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
	sge::renderer::device_ptr const renderer_;
	world &world_;
	std::unique_ptr<btBoxShape> chassis_box_;
	std::unique_ptr<btCompoundShape> compound_;
	std::unique_ptr<btRigidBody> car_body_;
	std::unique_ptr<btCylinderShapeX> wheel_shape_;
	std::unique_ptr<btVehicleRaycaster> raycaster_; 
	std::unique_ptr<btRaycastVehicle> vehicle_;
	wheel_info_sequence const wheels_;
	scalar max_engine_force_;
	scalar max_breaking_force_;
	scalar max_speed_;
	scalar current_engine_force_;
	scalar current_breaking_force_;
	scalar current_steering_;
	scalar steering_clamp_;
	model::object_ptr chassis_model_;
	model::object_ptr wheel_model_;
	btTransform transform_;
	graphics::mat4 matrix_transform_;
	std::unique_ptr<upright_constraint> constraint_;

	bool is_skidding_;
	scoped_body world_body_scope_;
	scoped_action world_vehicle_scope_;

	// @override
	void
	getWorldTransform(
		btTransform &) const;

	// @override
	void
	setWorldTransform(
		btTransform const &);
};
}
}
}

#endif
