#include "scoped_constraint.hpp"
#include "world.hpp"
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>

insula::physics::scoped_constraint::scoped_constraint(
	world &_world,
	btTypedConstraint &_constraint)
:
	world_(
		_world),
	constraint_(
		_constraint)
{
	world_.add(
		constraint_);
}

insula::physics::scoped_constraint::~scoped_constraint()
{
	world_.remove(
		constraint_);
}
