#include "vehicle.hpp"
#include "dim3_to_bullet.hpp"
#include "vec3_to_bullet.hpp"
#include "bullet_to_vec3.hpp"
#include "bullet_to_mat3.hpp"
#include "transform_to_mat4.hpp"
#include "world.hpp"
#include "../graphics/box.hpp"
#include "../graphics/mat3.hpp"
#include "../gizmo/from_mat3.hpp"
#include "../model/object.hpp"
#include <LinearMath/btMatrix3x3.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletDynamics/Vehicle/btRaycastVehicle.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>
#include <BulletCollision/CollisionShapes/btCylinderShape.h>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/matrix/structure_cast.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <boost/foreach.hpp>

// DEBUG
#include <fcppt/io/cout.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/box/output.hpp>
#include "bullet_to_vec3.hpp"

namespace
{
// NOTE: There's also just btVehicleTuning, but I think that's a bug
btRaycastVehicle::btVehicleTuning const
create_tuning(
	insula::physics::wheel_info const &w)
{
	btRaycastVehicle::btVehicleTuning t;
	t.m_suspensionStiffness = w.suspension_stiffness();
	t.m_suspensionCompression = w.suspension_compression();
	t.m_suspensionDamping = w.suspension_damping();
	t.m_frictionSlip = w.friction_slip();
	t.m_maxSuspensionForce = w.maximum_suspension_force();
	return t;
}
}

insula::physics::vehicle::vehicle(
	world &_world,
	sge::renderer::device_ptr const _renderer,
	model::object_ptr _chassis_model,
	scalar const mass,
	scalar const chassis_position,
	scalar const _steering_clamp,
	vec3 const &_position,
	scalar const _max_engine_force,
	scalar const _max_breaking_force,
	model::object_ptr _wheel_model,
	wheel_info_sequence const &_wheels)
:
	renderer_(
		_renderer),
	world_(
		_world),
	wheels_(
		_wheels),
	max_engine_force_(
		_max_engine_force),
	max_breaking_force_(
		_max_breaking_force),
	current_engine_force_(
		0),
	current_breaking_force_(
		0),
	current_steering_(
		0),
	steering_clamp_(
		_steering_clamp),
	chassis_model_(
		_chassis_model),
	wheel_model_(
		_wheel_model),
	transform_(
		btMatrix3x3::getIdentity(),
		vec3_to_bullet(
			_position))
{
	setWorldTransform(
		transform_);
	
	chassis_box_.reset(
		new btBoxShape(
			dim3_to_bullet(
				fcppt::math::dim::structure_cast<dim3>(
					chassis_model_->bounding_box().dimension()))));
	
	compound_.reset(
		new btCompoundShape());

	compound_->addChildShape(
		btTransform(
			btMatrix3x3::getIdentity(),
			btVector3(
				0,
				chassis_position,
				0)),
		chassis_box_.get());

	btVector3 local_inertia;
	compound_->calculateLocalInertia(
		mass,
		local_inertia);

	car_body_.reset(
		new btRigidBody(
			btRigidBody::btRigidBodyConstructionInfo(
				mass,
				this,
				compound_.get(),
				local_inertia)));

	world_.add(
		*car_body_);

	// TODO: What happens if this is omitted?
	car_body_->setActivationState(
		DISABLE_DEACTIVATION);

	graphics::box const wheel_box = 
		wheel_model_->bounding_box();

	scalar const 
		wheel_halfwidth = 
			static_cast<scalar>(
				wheel_box.w()/2),
		wheel_radius = 
			static_cast<scalar>(
				wheel_box.h()/2);

	wheel_shape_.reset(
		new btCylinderShapeX(
			btVector3(
				wheel_halfwidth,
				wheel_radius,
				wheel_radius)));

	raycaster_.reset(
		new btDefaultVehicleRaycaster(
			&(world_.handle())));

	vehicle_.reset(
		new btRaycastVehicle(
			// bullet doesn't need this structure here, it's probably there for
			// historical reasons
			btRaycastVehicle::btVehicleTuning(),
			car_body_.get(),
			raycaster_.get()));

	// Those are just indices
	vehicle_->setCoordinateSystem(
		0,
		1,
		2);

	world_.add(
		*vehicle_);

	int wheel_id = 0;
	BOOST_FOREACH(
		wheel_info const &w,
		wheels_)
	{
		vehicle_->addWheel(
			vec3_to_bullet(
				w.position()),
			// Turning direction
			btVector3(
				0,-1,0),
			// Axle
			btVector3(
				-1,0,0),
			w.suspension_rest_length(),
			wheel_radius,
			create_tuning(
				w),
			w.is_front_wheel());
		btWheelInfo &btwheel = 
			vehicle_->getWheelInfo(
				wheel_id++);

		btwheel.m_suspensionStiffness = w.suspension_stiffness();
		btwheel.m_wheelsDampingRelaxation = w.damping_relaxation();
		btwheel.m_wheelsDampingCompression = w.damping_compression();
		btwheel.m_frictionSlip = w.friction_slip();
		btwheel.m_rollInfluence = w.roll_influence();
	}
}

void
insula::physics::vehicle::update()
{
	for (wheel_info_sequence::size_type i = 0; i < wheels_.size(); ++i)
	{
		if (wheels_[i].gets_engine_force())
		{
			vehicle_->applyEngineForce(
				current_engine_force_,
				//max_engine_force_,
				static_cast<int>(
					i));
		}

		if (wheels_[i].gets_breaking_force())
			vehicle_->setBrake(
				current_breaking_force_,
				static_cast<int>(
					i));

		if (wheels_[i].gets_steering())
			vehicle_->setSteeringValue(
				current_steering_,
				static_cast<int>(
					i));

		vehicle_->updateWheelTransform(
			static_cast<int>(
				i),
			true);
	}
}

void
insula::physics::vehicle::render()
{
	for (wheel_info_sequence::size_type i = 0; i < wheels_.size(); ++i)
	{
		wheel_model_->render(
			transform_to_mat4(
				vehicle_->getWheelInfo(static_cast<int>(i)).m_worldTransform));
	}

	chassis_model_->render(
		matrix_transform_);
}

void
insula::physics::vehicle::engine_force(
	scalar const v)
{
	current_engine_force_ = v * max_engine_force_;
}

void
insula::physics::vehicle::breaking_force(
	scalar const v)
{
	current_breaking_force_ = v * max_breaking_force_;
}

void
insula::physics::vehicle::steering(
	scalar const s)
{
	current_steering_ = s * steering_clamp_;
}

insula::physics::gizmo const
insula::physics::vehicle::gizmo() const
{
	insula::physics::gizmo g = 
		insula::gizmo::from_mat3<scalar>(
			bullet_to_mat3(
				transform_.getBasis()));
	g.position(
		bullet_to_vec3(
			transform_.getOrigin()));
	return g;
}

insula::physics::vehicle::~vehicle()
{
	world_.remove(
		*vehicle_);

	world_.remove(
		*car_body_);
}

void
insula::physics::vehicle::getWorldTransform(
	btTransform &t) const
{
	t = transform_;
}

// @override
void
insula::physics::vehicle::setWorldTransform(
	btTransform const &t)
{
	transform_ = t;

	matrix_transform_ = 
		transform_to_mat4(
			t);
}