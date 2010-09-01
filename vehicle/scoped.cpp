#include "scoped.hpp"
#include "object.hpp"

insula::vehicle::scoped::scoped(
	object &_object)
:
	object_(
		_object)
{
	object_.activate();
}

insula::vehicle::scoped::~scoped()
{
	object_.deactivate();
}
