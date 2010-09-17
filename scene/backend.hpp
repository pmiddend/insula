#ifndef INSULA_SCENE_BACKEND_HPP_INCLUDED
#define INSULA_SCENE_BACKEND_HPP_INCLUDED

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
	backend();

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
