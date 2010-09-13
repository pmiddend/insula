#ifndef INSULA_PHYSICS_WORLD_HPP_INCLUDED
#define INSULA_PHYSICS_WORLD_HPP_INCLUDED

#include "box.hpp"
#include "vec3.hpp"
#include "iteration.hpp"
#include "vehicle_static_callback.hpp"
#include "object_fwd.hpp"
#include "filter_callback.hpp"
#include "../time_delta.hpp"
#include "../graphics/camera/object_fwd.hpp"
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/object.hpp>
#include <memory>
#include <set>

class btCollisionConfiguration;
class btCollisionObject;
class btDispatcher;
//class btBroadphaseInterface;
class btDbvtBroadphase;
class btConstraintSolver;
class btDynamicsWorld;
class btRigidBody;
class btActionInterface;

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
		graphics::camera::object &,
		box const &world_size,
		vec3 const &gravity);

	void
	update(
		time_delta);

	void
	update_visibility();

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

	// The raycast vehicle needs this in the constructor
	btDynamicsWorld &
	handle();

	iteration 
	current_iteration() const;

	fcppt::signal::auto_connection
	register_vehicle_static_callback(
		vehicle_static_callback const &);

	// Has to be there because of the destructors of incomplete types
	~world();
private:
	typedef
	std::set<std::pair<object *,object *>>
	contact_set;

	graphics::camera::object &camera_;
	fcppt::signal::object<vehicle_static_callback_fn> vehicle_static_signal_;
	std::unique_ptr<btCollisionConfiguration> configuration_;
	std::unique_ptr<btDispatcher> dispatcher_;
	// We cannot use btBroadphaseInterface anymore because we explicitly
	// want a dbvt broadphase for frustum culling
	//std::unique_ptr<btBroadphaseInterface> broadphase_;
	std::unique_ptr<btDbvtBroadphase> broadphase_;
	std::unique_ptr<btConstraintSolver> constraint_solver_;
	std::unique_ptr<btDynamicsWorld> world_;
	filter_callback filter_callback_;
	contact_set contacts_;
	iteration current_iteration_;

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
