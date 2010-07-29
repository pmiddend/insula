#ifndef INSULA_GRAPHICS_CAMERA_HPP_INCLUDED
#define INSULA_GRAPHICS_CAMERA_HPP_INCLUDED

#include "scalar.hpp"
#include "mat4.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "gizmo.hpp"
#include "../console/object_fwd.hpp"
#include <sge/input/system_ptr.hpp>
#include <sge/input/key_pair_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/math/vector/basic_impl.hpp>

namespace insula
{
namespace graphics
{
class camera
{
FCPPT_NONCOPYABLE(camera)
public:
	explicit
	camera(
		console::object &,
		scalar aspect,
		scalar fov,
		scalar near,
		scalar far,
		scalar camera_speed,
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

	// NOTE: Is it OK to return by reference here?
	vec3 const &
	position() const;

	// NOTE: Is it OK to return by reference here?
	gizmo const &
	axes() const;

	void
	axes(
		gizmo const &);
private:
	fcppt::signal::scoped_connection input_connection_;
	scalar aspect_,fov_,near_,far_;
	scalar speed_;
	vec3 dirs_;
	vec3 position_;
	gizmo axes_;
	//vec4 forward_,right_,up_;
	scalar do_roll_,roll_speed_;

	void
	input_callback(
		sge::input::key_pair const &);
};
}
}

#endif
