#include "world.hpp"
#include "vec3_to_bullet.hpp"
#include "object.hpp"
#include "height_map.hpp"
#include "broadphase/manager.hpp"
#include "../graphics/frustum.hpp"
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

insula::physics::world::world(
	box const &/*world_size*/,
	vec3 const &gravity,
	broadphase::manager &broadphase_manager)
:
	configuration_(
		new btDefaultCollisionConfiguration()),
	dispatcher_(
		new btCollisionDispatcher(
			configuration_.get())),
	broadphase_(
		broadphase_manager.create()),
	constraint_solver_(
		new btSequentialImpulseConstraintSolver()),
	world_(
		new btDiscreteDynamicsWorld(
			dispatcher_.get(),
			broadphase_.get(),
			constraint_solver_.get(),
			configuration_.get())),
	filter_callback_(
		*world_->getPairCache())
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
		3);
	//		100,
	//		static_cast<btScalar>(1.0/240.0));

	process_collisions();
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

void
insula::physics::world::add(
	btTypedConstraint &b)
{
	world_->addConstraint(
		&b);
}

void
insula::physics::world::remove(
	btTypedConstraint &b)
{
	world_->removeConstraint(
		&b);
}

btDiscreteDynamicsWorld &
insula::physics::world::handle()
{
	return *world_;
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

template<typename A,typename B,typename Base,typename SignalMap>
bool
try_combination(
	Base *a,
	Base *b,
	SignalMap &signals)
{
	A *ca = dynamic_cast<A *>(a);
	B *cb = dynamic_cast<B *>(b);

	if (ca && cb)
	{
		signals[std::minmax(ca->type(),cb->type())](
			std::ref(*ca),
			std::ref(*cb));
		return true;
	}

	ca = dynamic_cast<A *>(b);
	cb = dynamic_cast<B *>(a);

	if (ca && cb)
	{
		signals[std::minmax(ca->type(),cb->type())](
			std::ref(*ca),
			std::ref(*cb));
		return true;
	}

	return false;
}

void
insula::physics::world::process_collisions()
{
	BOOST_FOREACH(
		contact_set::const_reference contact,
		contacts_)
	{
	//	if (try_combination<vehicle::object,static_model>(contact.first,contact.second,signals_))
	//		continue;

	//	if (try_combination<vehicle::object,height_map>(contact.first,contact.second,signals_))
	//		continue;
		/*
		// vehicle <-> static model
		vehicle::object *v = dynamic_cast<vehicle::object *>(contact.first);
		static_model *sm = dynamic_cast<static_model *>(contact.second);

		if (v && sm)
		{
			signals_[std::minmax(v->type(),sm->type())](
				std::ref(*v),
				std::ref(*sm));
			continue;
		}

		v = dynamic_cast<vehicle::object *>(contact.second);
		sm = dynamic_cast<static_model *>(contact.first);

		if (v && sm)
		{
			signals_[std::minmax(v->type(),sm->type())](
				std::ref(*v),
				std::ref(*sm));
			continue;
		}
		*/
	}
}
