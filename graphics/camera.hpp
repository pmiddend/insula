#ifndef INSULA_GRAPHICS_CAMERA_HPP_INCLUDED
#define INSULA_GRAPHICS_CAMERA_HPP_INCLUDED

#include "scalar.hpp"
#include "mat4.hpp"
#include "vec3.hpp"
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
		sge::input::system_ptr,
		scalar aspect,
		scalar fov,
		scalar near,
		scalar far);
	
	void
	time_delta(
		scalar);
	
	mat4 const
	world() const;

	mat4 const
	perspective() const;
private:
	fcppt::signal::scoped_connection input_connection_;
	scalar time_delta_;
	scalar aspect_,fov_,near_,far_;
	scalar rotate_x_,rotate_y_;
	vec3 position_;

	void
	input_callback(
		sge::input::key_pair const &);
};
}
}

#endif
