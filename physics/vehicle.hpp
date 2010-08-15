#ifndef INSULA_PHYSICS_VEHICLE_HPP_INCLUDED
#define INSULA_PHYSICS_VEHICLE_HPP_INCLUDED

#include "../model/object_ptr.hpp"
#include "gizmo.hpp"
#include "../graphics/mat4.hpp"
#include "../graphics/vec3.hpp"
#include "upright_constraint.hpp"
#include "wheel_info_sequence.hpp"
#include "vec3.hpp"
#include "world_fwd.hpp"
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

//class btGeneric6DofConstraint;

namespace insula
{
namespace physics
{
class vehicle
:
	public btMotionState
{
public:
	vehicle(vehicle const &) = delete;
	vehicle &operator=(vehicle const &) = delete;
	
	explicit
	vehicle(
		world &,
		sge::renderer::device_ptr,
		model::object_ptr chassis,
		scalar mass,
		scalar chassis_position,
		scalar steering_clamp,
		vec3 const &position,
		scalar max_engine_force,
		scalar max_breaking_force,
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

	// The camera needs this
	insula::physics::gizmo const
	gizmo() const;

	~vehicle();
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
	scalar current_engine_force_;
	scalar current_breaking_force_;
	scalar current_steering_;
	scalar steering_clamp_;
	model::object_ptr chassis_model_;
	model::object_ptr wheel_model_;
	btTransform transform_;
	graphics::mat4 matrix_transform_;
	std::unique_ptr<upright_constraint> constraint_;
	//std::unique_ptr<btGeneric6DofConstraint> constraint_;

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

#endif
