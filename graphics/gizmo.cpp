#include "gizmo.hpp"

insula::graphics::gizmo::gizmo(
	gizmo_init const &init)
:
	forward_(
		init.forward()),
	right_(
		init.right()),
	up_(
		init.up())
{
}

insula::graphics::vec3 const &
insula::graphics::gizmo::forward() const
{
	return forward_;
}

insula::graphics::vec3 const &
insula::graphics::gizmo::right() const
{
	return right_;
}

insula::graphics::vec3 const &
insula::graphics::gizmo::up() const
{
	return up_;
}
