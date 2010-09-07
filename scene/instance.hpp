#ifndef INSULA_SCENE_INSTANCE_HPP_INCLUDED
#define INSULA_SCENE_INSTANCE_HPP_INCLUDED

#include "auto_unlink_hook.hpp"
#include "backend_fwd.hpp"

namespace insula
{
namespace scene
{
class instance
:
	public auto_unlink_hook
{
public:
	instance(instance const &) = delete;
	instance &operator=(instance const &) = delete;
	instance() = default;

	virtual void
	render(
		backend &) = 0;

	virtual bool
	is_visible() const = 0;

	virtual
	~instance();
};
}
}

#endif
