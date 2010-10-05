#ifndef INSULA_SCENE_BACKEND_HPP_INCLUDED
#define INSULA_SCENE_BACKEND_HPP_INCLUDED

#include "manager_fwd.hpp"
#include "backend_priority.hpp"

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

	explicit
	backend(
		manager &,
		backend_priority::type = backend_priority::normal);

	virtual void
	begin() = 0;

	virtual void
	end() = 0;

	backend_priority::type
	priority() const;

	virtual
	~backend();
private:
	manager &manager_;
	backend_priority::type const priority_;
};
}
}

#endif
