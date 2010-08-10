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

namespace insula
{
namespace graphics
{
namespace camera
{
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
		scalar camera_speed,
		scalar roll_speed,
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
	scalar speed_;
	vec3 dirs_;
	vec3 position_;
	insula::graphics::gizmo gizmo_;
	scalar do_roll_,roll_speed_;

	void
	input_callback(
		sge::input::key_pair const &);
};
}
}
}

#endif
