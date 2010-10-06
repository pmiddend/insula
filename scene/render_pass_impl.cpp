#include "render_pass_impl.hpp"
#include "manager.hpp"

insula::scene::render_pass_impl::render_pass_impl(
	manager &_manager,
	render_pass::type const _rp,
	render_pass_callback const &_callback)
:
	manager_(_manager),
	rp_(_rp)
{
	manager_.add(
		rp_,
		_callback);
}

insula::scene::render_pass_impl::~render_pass_impl()
{
	manager_.remove(
		rp_);
}
