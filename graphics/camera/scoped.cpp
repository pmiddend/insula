#include "scoped.hpp"
#include "object.hpp"

insula::graphics::camera::scoped::scoped(
	object &_camera,
	vec3 const &new_position,
	gizmo const &new_gizmo)
:
	camera_(
		_camera),
	// Save the old state...
	position_(
		camera_.position()),
	gizmo_(
		camera_.axes())
{
	// ... and set the new state
	camera_.position(
		new_position);
	camera_.axes(
		new_gizmo);
}

insula::graphics::camera::scoped::~scoped()
{
	camera_.axes(
		gizmo_);
	camera_.position(
		position_);
}
