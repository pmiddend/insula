#ifndef INSULA_GRAPHICS_CAMERA_OBJECT_HPP_INCLUDED
#define INSULA_GRAPHICS_CAMERA_OBJECT_HPP_INCLUDED

#include "../scalar.hpp"
#include "../mat4.hpp"
#include "../vec3.hpp"
#include "../vec4.hpp"
#include "../gizmo.hpp"
#include "../../input_delegator_fwd.hpp"
#include <sge/input/system_ptr.hpp>
#include <sge/input/key_pair_fwd.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/math/vector/basic_impl.hpp>

#include <fcppt/io/cout.hpp>

namespace insula
{
namespace graphics
{
namespace camera
{
/// The camera is a class which holds a gizmo (the camera position and
/// orientation) and manipulates it via keyboard and mouse input. It
/// also offers all of the transformation matrices to be used in the
/// shaders, including the perspective matrix (the other ones you
/// could derive from the gizmo).
class object
{
public:
	object(object const &) = delete;
	object &operator=(object const &) = delete;

	explicit
	object(
		input_delegator &,
		scalar aspect,
		scalar fov,
		scalar near,
		scalar far,
		scalar movement_speed,
		scalar rotation_speed,
		vec3 const &position);
	
	void
	update(
		scalar);
	
	mat4 const
	world() const;

	mat4 const
	rotation() const;

	mat4 const
	translation() const;

	mat4 const
	perspective() const;

	mat4 const 
	mvp() const;

	insula::graphics::gizmo const &
	gizmo() const;

	insula::graphics::gizmo &
	gizmo();

	graphics::scalar 
	aspect() const;

	graphics::scalar 
	fov() const;
private:
	fcppt::signal::scoped_connection input_connection_;
	scalar aspect_,fov_,near_,far_;
	scalar movement_speed_,rotation_speed_;
	vec3 dirs_;
	insula::graphics::gizmo gizmo_;

	void
	input_callback(
		sge::input::key_pair const &);
};
}
}
}

#endif
