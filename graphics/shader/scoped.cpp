#include "scoped.hpp"
#include "object.hpp"

insula::graphics::shader::scoped::scoped(
	object &_object)
:
	object_(
		_object)
{
	object_.activate();
}

insula::graphics::shader::scoped::~scoped()
{
	object_.deactivate();
}
