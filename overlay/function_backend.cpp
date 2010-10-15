#include "function_backend.hpp"
#include <boost/foreach.hpp>

insula::overlay::function_backend::function_backend(
	scene::manager &_manager,
	scene::backend::render_pass_set const &_render_passes,
	scene::backend_priority::type const _priority)
:
	scene::backend(
		_manager,
		_render_passes,
		_priority)
{
}

void 
insula::overlay::function_backend::begin(
	scene::render_pass::object const &)
{
	callback_();
}

void 
insula::overlay::function_backend::end(
	scene::render_pass::object const &)
{
}

fcppt::signal::auto_connection
insula::overlay::function_backend::register_callback(
	callback const &c)
{
	return callback_.connect(c);
}
