#include "backend.hpp"
#include "manager.hpp"

insula::scene::backend::backend(
	manager &_manager,
	render_pass_set const &_render_passes,
	backend_priority::type const _priority)
:
	manager_(
		_manager),
	render_passes_(
		_render_passes),
	priority_(
		_priority)
{
	manager_.add(
		*this);
}

bool
insula::scene::backend::takes_part_in(
	render_pass::identifier const &r) const
{
	return render_passes_.find(r) != render_passes_.end();
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
