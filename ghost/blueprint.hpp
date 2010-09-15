#ifndef INSULA_GHOST_BLUEPRINT_HPP_INCLUDED
#define INSULA_GHOST_BLUEPRINT_HPP_INCLUDED

#include "../model/backend_fwd.hpp"
#include "../physics/vec3.hpp"
#include "../physics/scalar.hpp"
#include <fcppt/math/vector/basic_impl.hpp>

namespace insula
{
namespace ghost
{
struct blueprint
{
public:
	model::backend &backend;
	physics::vec3 rotation_axis;
	physics::scalar rotation_angle;
	physics::scalar scaling;
	physics::vec3 origin;

	explicit
	blueprint(
		model::backend &backend,
		physics::vec3 rotation_axis,
		physics::scalar rotation_angle,
		physics::scalar scaling,
		physics::vec3 origin)
	:
		backend(backend),
		rotation_axis(rotation_axis),
		rotation_angle(rotation_angle),
		scaling(scaling),
		origin(origin)
	{
	}
};
}
}

#endif
