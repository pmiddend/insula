#include "scoped.hpp"
#include "object.hpp"

insula::graphics::camera::scoped::scoped(
	object &_camera,
	gizmo const &new_gizmo)
:
	camera_(
		_camera),
	gizmo_(
		camera_.gizmo())
{
	// ... and set the new state
	camera_.gizmo() = new_gizmo; 
}

insula::graphics::camera::scoped::~scoped()
{
	camera_.gizmo() = gizmo_;
}
