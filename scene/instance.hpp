#ifndef INSULA_SCENE_INSTANCE_HPP_INCLUDED
#define INSULA_SCENE_INSTANCE_HPP_INCLUDED

#include "auto_unlink_hook.hpp"

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

	virtual void
	render() = 0;

	virtual
	~instance();
};
}
}

#endif
