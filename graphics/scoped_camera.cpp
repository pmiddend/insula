#include "scoped_camera.hpp"
#include "camera.hpp"

insula::graphics::scoped_camera::scoped_camera(
	insula::graphics::camera &_camera,
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

insula::graphics::scoped_camera::~scoped_camera()
{
	camera_.axes(
		gizmo_);
	camera_.position(
		position_);
}
