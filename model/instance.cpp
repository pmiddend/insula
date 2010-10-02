#include "instance.hpp"
#include "backend.hpp"
#include "object.hpp"
#include "../graphics/shader/object.hpp"
#include "../graphics/camera/object.hpp"
#include "../graphics/mat4.hpp"
#include <fcppt/math/matrix/arithmetic.hpp>

insula::model::instance::instance(
	graphics::mat4 const &_transformation)
:
	transformation_(
		_transformation)
{
}

void
insula::model::instance::render(
	scene::backend &back)
{
	model::backend &realback = 
		dynamic_cast<model::backend &>(
			back);

	realback.modelview(
		realback.camera().world() 
			* transformation_);

	realback.model().render();
}

bool
insula::model::instance::is_visible() const
{
	return true;
}

insula::graphics::scalar
insula::model::instance::distance_to(
	graphics::vec3 const &) const
{
	return 
		static_cast<graphics::scalar>(
			0);
}

void
insula::model::instance::transformation(
	graphics::mat4 const &_transformation)
{
	transformation_ = _transformation;
}
