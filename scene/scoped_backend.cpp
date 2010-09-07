#include "scoped_backend.hpp"
#include "backend.hpp"

insula::scene::scoped_backend::scoped_backend(
	backend_ptr const _backend)
:
	backend_(
		_backend)
{
	backend_->begin();
}

insula::scene::scoped_backend::~scoped_backend()
{
	backend_->end();
}
