#ifndef INSULA_SCENE_BACKEND_HPP_INCLUDED
#define INSULA_SCENE_BACKEND_HPP_INCLUDED

#include "auto_unlink_hook.hpp"

namespace insula
{
namespace scene
{
class backend
:
	public auto_unlink_hook
{
public:
	backend(backend const &) = delete;
	backend &operator=(backend const &) = delete;
	backend() = default;

	virtual void
	begin() = 0;

	virtual void
	end() = 0;

	virtual
	~backend();
};
}
}

#endif
