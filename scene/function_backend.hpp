#ifndef INSULA_SCENE_FUNCTION_BACKEND_HPP_INCLUDED
#define INSULA_SCENE_FUNCTION_BACKEND_HPP_INCLUDED

#include "manager_fwd.hpp"
#include "backend_priority.hpp"
#include "backend.hpp"
#include <fcppt/signal/object.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <functional>

namespace insula
{
namespace scene
{
class function_backend
:
	public backend
{
public:
	typedef
	void 
	callback_fn(
		render_pass::object const &);

	typedef
	std::function<callback_fn>
	callback;

	explicit
	function_backend(
		manager &,
		backend::render_pass_set const &,
		backend_priority::type = backend_priority::normal);

	void 
	begin(
		render_pass::object const &);

	void 
	end(
		render_pass::object const &);

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
