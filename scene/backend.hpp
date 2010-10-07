#ifndef INSULA_SCENE_BACKEND_HPP_INCLUDED
#define INSULA_SCENE_BACKEND_HPP_INCLUDED

#include "manager_fwd.hpp"
#include "backend_priority.hpp"
#include "render_pass/object_fwd.hpp"
#include "render_pass/identifier.hpp"
#include <set>

namespace insula
{
namespace scene
{
/**
	See scene::manager for a detailed background explanation.
 */
class backend
{
public:
	backend(backend const &) = delete;
	backend &operator=(backend const &) = delete;

	typedef
	std::set<render_pass::identifier>
	render_pass_set;

	explicit
	backend(
		manager &,
		render_pass_set const &,
		backend_priority::type = backend_priority::normal);

	bool
	takes_part_in(
		render_pass::identifier const &) const;

	virtual void
	begin(
		render_pass::object const &) = 0;

	virtual void
	end(
		render_pass::object const &) = 0;

	backend_priority::type
	priority() const;

	virtual
	~backend();
private:
	manager &manager_;
	render_pass_set const render_passes_;
	backend_priority::type const priority_;
};
}
}

#endif
