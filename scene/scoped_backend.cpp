#include "scoped_backend.hpp"
#include "backend.hpp"

insula::scene::scoped_backend::scoped_backend(
	backend_ptr const _backend,
	render_pass::object const &_render_pass)
:
	backend_(
		_backend),
	render_pass_(
		_render_pass)
{
	backend_->begin(
		render_pass_);
}

insula::scene::scoped_backend::~scoped_backend()
{
	backend_->end(
		render_pass_);
}
