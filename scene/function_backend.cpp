#include "function_backend.hpp"
#include <boost/foreach.hpp>

insula::scene::function_backend::function_backend(
	manager &_manager,
	backend::render_pass_set const &_render_passes,
	backend_priority::type const _priority)
:
	backend(
		_manager,
		_render_passes,
		_priority)
{
}

void 
insula::scene::function_backend::begin(
	render_pass::object const &r)
{
	callback_(
		r);
}

void 
insula::scene::function_backend::end(
	render_pass::object const &)
{
}

fcppt::signal::auto_connection
insula::scene::function_backend::register_callback(
	callback const &c)
{
	return callback_.connect(c);
}
