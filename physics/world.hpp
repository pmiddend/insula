#ifndef INSULA_PHYSICS_WORLD_HPP_INCLUDED
#define INSULA_PHYSICS_WORLD_HPP_INCLUDED

#include "box.hpp"
#include "vec3.hpp"
#include "callback_fn.hpp"
#include "object_fwd.hpp"
#include "filter_callback.hpp"
#include "trampoline.hpp"
#include "object_type.hpp"
#include "broadphase/shared_object_ptr.hpp"
#include "broadphase/manager_fwd.hpp"
#include "../time_delta.hpp"
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/object.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <memory>
#include <algorithm>
#include <set>

class btCollisionConfiguration;
class btCollisionObject;
class btDispatcher;
//class btBroadphaseInterface;
class btDbvtBroadphase;
class btConstraintSolver;
class btDiscreteDynamicsWorld;
class btDynamicsWorld;
class btRigidBody;
class btActionInterface;
class btTypedConstraint;

namespace insula
{
namespace physics
{
class world
{
public:
	world(world const &) = delete;
	world &operator=(world const &) = delete;

	explicit
	world(
		box const &world_size,
		vec3 const &gravity,
		broadphase::manager &);

	void
	update(
		time_delta);

	void
	add(
		btRigidBody &);

	void
	remove(
		btRigidBody &);

	void
	add(
		btActionInterface &);

	void
	remove(
		btActionInterface &);

	void
	add(
		btTypedConstraint &);

	void
	remove(
		btTypedConstraint &);

	// The debug drawer needs this (might be replaced by a better
	// function)
	btDiscreteDynamicsWorld &
	handle();
	/**
		Some explanation is needed here:
		
		The physics itself offers several different types of objects:

		- terrain
		- static_model

		Those are C++ types, and we want collision signals for each pair
		those types. _But_ that's not enough, since we have objects with
		the same C++ type but which are distinct, like a nugget and a
		static model (a palm tree, for example). So we invented
		physics::object_type. Two static_models can now have the types
		physics::nugget and physics::prop. The object_type and the physics
		type are really orthogonal. 

		So we have a template function here because we want callbacks with
		type "void (A first_object,B second_object)", and we also want to
		filter out specific object_types. To avoid issues with the order
		of the signal_map we use minmax here. The trampoline type
		dynamic-downcasts the objects accordingly. No real magic done
		here.
	 */
	template<typename A,typename B,typename F>
	fcppt::signal::auto_connection
	register_callback(
		object_type::type const a,
		object_type::type const b,
		F const &f)
	{
		return 
			signals_[std::minmax(a,b)].connect(
				trampoline<A,B>(
					f));
	}

	// Has to be there because of the destructors of incomplete types
	~world();
private:
	typedef
	std::set<std::pair<object *,object *>>
	contact_set;

	typedef
	boost::ptr_map
	<
		std::pair<object_type::type,object_type::type>,
		fcppt::signal::object<callback_fn>
	>
	signal_map;

	signal_map signals_;
	std::unique_ptr<btCollisionConfiguration> configuration_;
	std::unique_ptr<btDispatcher> dispatcher_;
	broadphase::shared_object_ptr broadphase_;
	std::unique_ptr<btConstraintSolver> constraint_solver_;
	std::unique_ptr<btDiscreteDynamicsWorld> world_;
	filter_callback filter_callback_;
	contact_set contacts_;

	static void
	static_tick_callback(
		btDynamicsWorld *world, 
		btScalar timeStep);

	void
	tick_callback(
		btScalar);

	void
	process_collisions();
};
}
}

#endif
