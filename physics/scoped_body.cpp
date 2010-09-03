#include "scoped_body.hpp"
#include "world.hpp"

insula::physics::scoped_body::scoped_body(
	world &w,
	btRigidBody &b)
:
	w(w),
	b(b)
{
	w.add(
		b);
}

insula::physics::scoped_body::~scoped_body()
{
	w.remove(
		b);
}
