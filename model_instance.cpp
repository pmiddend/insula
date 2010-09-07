#include "model_instance.hpp"
#include "model_backend.hpp"
#include "graphics/shader/object.hpp"
#include "graphics/camera/object.hpp"
#include "model/object.hpp"
#include "graphics/mat4.hpp"
#include <fcppt/math/matrix/arithmetic.hpp>

insula::model_instance::model_instance(
	graphics::mat4 const &_transformation)
:
	transformation_(
		_transformation)
{
}

void
insula::model_instance::render(
	scene::backend &back)
{
	model_backend &realback = 
		dynamic_cast<model_backend &>(
			back);

	realback.shader().set_uniform(
		"mvp",
		realback.camera().mvp() * 
		transformation_);

	realback.model().render();
}

bool
insula::model_instance::is_visible() const
{
	return true;
}

insula::graphics::scalar
insula::model_instance::distance_to(
	graphics::vec3 const &) const
{
	return 
		static_cast<graphics::scalar>(
			0);
}
