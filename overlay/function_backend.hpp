#ifndef INSULA_OVERLAY_FUNCTION_BACKEND_HPP_INCLUDED
#define INSULA_OVERLAY_FUNCTION_BACKEND_HPP_INCLUDED

#include "../scene/manager_fwd.hpp"
#include "../scene/backend_priority.hpp"
#include "../scene/backend.hpp"
#include <fcppt/signal/object.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <functional>

namespace insula
{
namespace overlay
{
class function_backend
:
	public scene::backend
{
public:
	typedef
	void 
	callback_fn();

	typedef
	std::function<callback_fn>
	callback;

	explicit
	function_backend(
		scene::manager &,
		scene::backend::render_pass_set const &,
		scene::backend_priority::type = scene::backend_priority::normal);

	void 
	begin(
		scene::render_pass::object const &);

	void 
	end(
		scene::render_pass::object const &);

	fcppt::signal::auto_connection
	register_callback(
		callback const &);
private:
	typedef
	fcppt::signal::object<callback_fn>
	callback_signal;

	callback_signal callback_;
};
}
}

#endif
