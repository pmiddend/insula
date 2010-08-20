#include "scoped_action.hpp"
#include "world.hpp"

insula::physics::scoped_action::scoped_action(
	world &w)
:
	w(w),
	a(0)
{
}

void
insula::physics::scoped_action::set(
	btActionInterface &_a)
{
	a = &_a;
	w.add(
		_a);
}

insula::physics::scoped_action::~scoped_action()
{
	if (a)
		w.remove(*a);
}
