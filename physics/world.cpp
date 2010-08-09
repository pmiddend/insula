#include "world.hpp"
#include "vec3_to_bullet.hpp"
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/BroadphaseCollision/btAxisSweep3.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/dim.hpp>

insula::physics::world::world(
	box const &world_size)
:
	configuration_(
		new btDefaultCollisionConfiguration()),
	dispatcher_(
		new btCollisionDispatcher(
			configuration_.get())),
	broadphase_interface_(
		new btAxisSweep3(
			vec3_to_bullet(
				world_size.pos()),
			vec3_to_bullet(
				world_size.pos() + world_size.dimension()))),
	constraint_solver_(
		new btSequentialImpulseConstraintSolver()),
	world_(
		new btDiscreteDynamicsWorld(
			dispatcher_.get(),
			broadphase_interface_.get(),
			constraint_solver_.get(),
			configuration_.get()))
{
	// bullet sets some default value, so we better override this here
	world_->setGravity(
		btVector3(
			static_cast<btScalar>(0),
			static_cast<btScalar>(-100),
			static_cast<btScalar>(0)));
}

void
insula::physics::world::update(
	scalar const time_delta)
{
	world_->stepSimulation(
		time_delta,
		// Maximum simulation substeps: The fixed time step is 1/60. We
		// have to choose the substep count so that time_delta is _just_
		// less than the fixed time.
		2);
}

void
insula::physics::world::add(
	btRigidBody &b)
{
	world_->addRigidBody(
		&b);
}

void
insula::physics::world::remove(
	btRigidBody &b)
{
	world_->removeRigidBody(
		&b);
}

void
insula::physics::world::add(
	btActionInterface &b)
{
	world_->addAction(
		&b);
}

void
insula::physics::world::remove(
	btActionInterface &b)
{
	world_->removeAction(
		&b);
}

btDynamicsWorld &
insula::physics::world::handle()
{
	return *world_;
}

insula::physics::world::~world()
{
}
