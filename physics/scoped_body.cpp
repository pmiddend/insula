#include "scoped_body.hpp"
#include "world.hpp"

insula::physics::scoped_body::scoped_body(
	world &w)
:
	w(w),
	b(0)
{
}

void 
insula::physics::scoped_body::set(
	btRigidBody &_b)
{
	b = &_b;
	w.add(
		_b);
}

insula::physics::scoped_body::~scoped_body()
{
	if (b)
		w.remove(
			*b);
}
