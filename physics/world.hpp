#ifndef INSULA_PHYSICS_WORLD_HPP_INCLUDED
#define INSULA_PHYSICS_WORLD_HPP_INCLUDED

#include "box.hpp"
#include "vec3.hpp"
#include <memory>

class btCollisionConfiguration;
class btDispatcher;
class btBroadphaseInterface;
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
		box const &world_size,
		vec3 const &gravity);

	void
	update(
		scalar time_delta);

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

	// Has to be there because of the destructors of incomplete types
	~world();
private:
	std::unique_ptr<btCollisionConfiguration> configuration_;
	std::unique_ptr<btDispatcher> dispatcher_;
	std::unique_ptr<btBroadphaseInterface> broadphase_interface_;
	std::unique_ptr<btConstraintSolver> constraint_solver_;
	std::unique_ptr<btDynamicsWorld> world_;
};
}
}

#endif
