#include "transparent_instance.hpp"
#include <fcppt/assert.hpp>

insula::scene::transparent_instance::transparent_instance()
:
	backend_(0)
{
}

insula::scene::backend_ptr
insula::scene::transparent_instance::backend() const
{
	return backend_;
}

void
insula::scene::transparent_instance::backend(
	backend_ptr const _backend)
{
	FCPPT_ASSERT(!backend_);
	backend_ = _backend;
}

insula::scene::transparent_instance::~transparent_instance() {}
