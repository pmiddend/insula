#ifndef INSULA_SCENE_TRANSPARENT_INSTANCE_HPP_INCLUDED
#define INSULA_SCENE_TRANSPARENT_INSTANCE_HPP_INCLUDED

#include "backend_ptr.hpp"
#include "instance.hpp"
#include "../graphics/vec3.hpp"
#include "../graphics/scalar.hpp"

namespace insula
{
namespace scene
{
class transparent_instance
:
	public instance
{
public:
	explicit
	transparent_instance();

	virtual graphics::scalar
	distance_to(
		graphics::vec3 const &) const = 0;

	backend_ptr
	backend() const;

	// gcc warns if it's missing but I don't really know why
	virtual
	~transparent_instance();
private:
	friend class manager;

	backend_ptr backend_;

	void
	backend(
		backend_ptr);
};
}
}

#endif
