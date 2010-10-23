#ifndef INSULA_SHADOW_UPDATE_CALLBACK_FN_HPP_INCLUDED
#define INSULA_SHADOW_UPDATE_CALLBACK_FN_HPP_INCLUDED

#include "../graphics/scalar.hpp"
#include "../graphics/gizmo.hpp"

namespace insula
{
namespace shadow
{
typedef
void
update_callback_fn(
	// sun angle
	graphics::scalar,
	// sun gizmo (position and orientation
	graphics::gizmo const &);
}
}

#endif
