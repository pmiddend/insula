#include "backend.hpp"
#include "manager.hpp"

insula::scene::backend::backend(
	manager &_manager,
	backend_priority::type const _priority)
:
	manager_(
		_manager),
	priority_(
		_priority)
{
	manager_.add(
		*this);
}

insula::scene::backend_priority::type
insula::scene::backend::priority() const
{
	return priority_;
}

insula::scene::backend::~backend() 
{
	manager_.remove(
		*this);
}
