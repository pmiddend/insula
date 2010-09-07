#ifndef INSULA_MODEL_INSTANCE_HPP_INCLUDED
#define INSULA_MODEL_INSTANCE_HPP_INCLUDED

#include "scene/transparent_instance.hpp"
#include "graphics/mat4.hpp"
#include "graphics/vec3.hpp"
#include "scene/backend_fwd.hpp"
#include "graphics/scalar.hpp"
#include <fcppt/math/matrix/basic_impl.hpp>

namespace insula
{
class model_instance
:
	public scene::transparent_instance
{
public:
	explicit
	model_instance(
		graphics::mat4 const &);

	void
	render(
		scene::backend &);

	// Lacking any "generic" visibility information, this function
	// always returns true
	bool
	is_visible() const;

	// This function will always return zero since a bounding box test
	// is not possible (we'd have to store the model somewhere or pass
	// it in the constructor)
	graphics::scalar
	distance_to(
		graphics::vec3 const &) const;

	void
	transformation(
		graphics::mat4 const &);
private:
	graphics::mat4 transformation_;
};
}

#endif
