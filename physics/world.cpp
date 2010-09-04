#include "world.hpp"
#include "vec3_to_bullet.hpp"
#include "object.hpp"
#include "culler.hpp"
#include "../graphics/frustum.hpp"
#include "../graphics/camera/object.hpp"
#include "vehicle/object.hpp"
#include "static_model.hpp"
#include "../math/extract_frustum.hpp"
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
//#include <BulletCollision/BroadphaseCollision/btAxisSweep3.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/NarrowPhaseCollision/btPersistentManifold.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/dim.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/io/cerr.hpp>
#include <boost/foreach.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <array>

insula::physics::world::world(
	graphics::camera::object &_camera,
	box const &/*world_size*/,
	vec3 const &gravity)
:
	camera_(
		_camera),
	configuration_(
		new btDefaultCollisionConfiguration()),
	dispatcher_(
		new btCollisionDispatcher(
			configuration_.get())),
	broadphase_(
		new btDbvtBroadphase()
		/*
		new btAxisSweep3(
			vec3_to_bullet(
				world_size.pos()),
			vec3_to_bullet(
				world_size.pos() + world_size.dimension()))*/),
	constraint_solver_(
		new btSequentialImpulseConstraintSolver()),
	world_(
		new btDiscreteDynamicsWorld(
			dispatcher_.get(),
			broadphase_.get(),
			constraint_solver_.get(),
			configuration_.get())),
	current_iteration_(
		// This is zero so it's different from the default iteration value
		// of the objects
		static_cast<iteration>(
			1))
{
	// bullet sets some default value, so we better override this here
	world_->setGravity(
		vec3_to_bullet(
			gravity));

	world_->setInternalTickCallback(
		&world::static_tick_callback,
		// This is the user info, retrievable via world->getWorldUserInfo()
		this);
}

void
insula::physics::world::update(
	time_delta const delta)
{
	contacts_.clear();

	//fcppt::io::cerr << "Stepping the simulation\n";

	world_->stepSimulation(
		static_cast<scalar>(
			delta),
		// Maximum simulation substeps: The fixed time step is 1/60. We
		// have to choose the substep count so that time_delta is _just_
		// less than the fixed time.
	//	3);
			3);
	//	100,
	//	static_cast<btScalar>(1.0/240.0));

	process_collisions();
}

void
insula::physics::world::add(
	btRigidBody &b)
{
	world_->addRigidBody(
		&b);
	//fcppt::io::cout << "Added body with broadphase address: " << b.getBroadphaseHandle() << "\n";
}

void
insula::physics::world::remove(
	btRigidBody &b)
{
	//fcppt::io::cerr << "Removing a rigid body with collision handle: " << b.getBroadphaseHandle() << "\n";
	world_->removeRigidBody(
		&b);
}

void
insula::physics::world::add(
	btActionInterface &b)
{
	world_->addAction(
		&b);
	world_->addVehicle(
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

insula::physics::iteration
insula::physics::world::current_iteration() const
{
	return current_iteration_;
}

fcppt::signal::auto_connection
insula::physics::world::register_vehicle_static_callback(
	vehicle_static_callback const &c)
{
	return vehicle_static_signal_.connect(c);
}

insula::physics::world::~world()
{
}

void
insula::physics::world::static_tick_callback(
	btDynamicsWorld *w, 
	btScalar const time_step)
{
	static_cast<physics::world *>(w->getWorldUserInfo())->tick_callback(
		time_step);
}

void
insula::physics::world::tick_callback(
	btScalar /*time_step*/)
{
	// Yeaaaaaah!
	BOOST_FOREACH(
		btPersistentManifold *manifold,
		boost::make_iterator_range(
			world_->getDispatcher()->getInternalManifoldPointer(),
			world_->getDispatcher()->getInternalManifoldPointer() + 
				world_->getDispatcher()->getNumManifolds()))
	{
		// Apparently, there can be manifolds which don't have contact points
		if (!manifold->getNumContacts())
			continue;

		object 
			&first = 
				*static_cast<object *>(
					static_cast<btCollisionObject*>(
						manifold->getBody0())->getUserPointer()),
			&second =
				*static_cast<object *>(
					static_cast<btCollisionObject*>(
						manifold->getBody1())->getUserPointer());
	
		contacts_.insert(
			contact_set::value_type(
				&first,
				&second));
		/*
		int numContacts = contactManifold->getNumContacts();
		for (int j=0;j<numContacts;j++)
		{
			btManifoldPoint& pt = contactManifold->getContactPoint(j);
			if (pt.getDistance()<0.f)
			{
				const btVector3& ptA = pt.getPositionWorldOnA();
				const btVector3& ptB = pt.getPositionWorldOnB();
				const btVector3& normalOnB = pt.m_normalWorldOnB;
			}
		}
		*/
	}
}

void
insula::physics::world::process_collisions()
{
	BOOST_FOREACH(
		contact_set::const_reference contact,
		contacts_)
	{
		vehicle::object *v = dynamic_cast<vehicle::object *>(contact.first);
		static_model *sm = dynamic_cast<static_model *>(contact.second);

		if (v && sm)
		{
			vehicle_static_signal_(
				std::ref(*v),
				std::ref(*sm));
			continue;
		}

		v = dynamic_cast<vehicle::object *>(contact.second);
		sm = dynamic_cast<static_model *>(contact.first);

		if (v && sm)
		{
			vehicle_static_signal_(
				std::ref(*v),
				std::ref(*sm));
			continue;
		}
	}
}

void
insula::physics::world::update_visibility()
{
	std::size_t const plane_count = 4;
	
	std::array<btVector3,plane_count> normals;
	std::array<scalar,plane_count> lambdas;

	graphics::frustum const f = 
		math::extract_frustum<graphics::scalar>(
			camera_.mvp());

	normals[0] = vec3_to_bullet(f.left().normal());
	normals[1] = vec3_to_bullet(f.right().normal());
	normals[2] = vec3_to_bullet(f.top().normal());
	normals[3] = vec3_to_bullet(f.bottom().normal());

	lambdas[0] = f.left().lambda();
	lambdas[1] = f.right().lambda();
	lambdas[2] = f.top().lambda();
	lambdas[3] = f.bottom().lambda();

	culler culler_(
		++current_iteration_);
	
	//fcppt::io::cout << "collide static\n";
	btDbvt::collideKDOP(
		broadphase_->m_sets[0].m_root,
		normals.data(),
		lambdas.data(),
		static_cast<int>(
			normals.size()),
		culler_);

	//fcppt::io::cout << "collide dynamic\n";
	btDbvt::collideKDOP(
		broadphase_->m_sets[1].m_root,
		normals.data(),
		lambdas.data(),
		static_cast<int>(
			normals.size()),
		culler_);
}
