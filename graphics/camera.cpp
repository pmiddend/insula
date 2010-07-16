#include "camera.hpp"
#include <sge/input/key_pair.hpp>
#include <sge/input/system.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/matrix/perspective.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/matrix/rotation_x.hpp>
#include <fcppt/math/matrix/rotation_y.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/output.hpp>
#include <fcppt/io/cout.hpp>

insula::graphics::camera::camera(
	sge::input::system_ptr const _is,
	scalar _aspect,
	scalar _fov,
	scalar _near,
	scalar _far)
:
	input_connection_(
		_is->register_callback(
			std::tr1::bind(
				&camera::input_callback,
				this,
				std::tr1::placeholders::_1))),
	time_delta_(
		static_cast<scalar>(0)),
	aspect_(
		_aspect),
	fov_(
		_fov),
	near_(
		_near),
	far_(
		_far),
	rotate_x_(
		static_cast<scalar>(
			0)),
	rotate_y_(
		static_cast<scalar>(
			0))
{
}

void
insula::graphics::camera::time_delta(
	scalar const _time_delta)
{
	time_delta_ = 
		_time_delta;
}

insula::graphics::mat4 const
insula::graphics::camera::world() const
{
	return 
		fcppt::math::matrix::rotation_x(
			rotate_x_) *
		fcppt::math::matrix::rotation_y(
			rotate_y_);
	
}

insula::graphics::mat4 const
insula::graphics::camera::perspective() const
{
	return fcppt::math::matrix::perspective(
			aspect_,
			fov_,
			near_,
			far_);
}

void
insula::graphics::camera::input_callback(
	sge::input::key_pair const &k)
{
	scalar const mouse_inverse_speed = 
		static_cast<scalar>(
			1000);

	switch (k.key().code())
	{
		case sge::input::kc::mouse_x_axis:
			rotate_y_ += 
				static_cast<scalar>(k.value())/mouse_inverse_speed;
			break;
		case sge::input::kc::mouse_y_axis:
			rotate_x_ += 
				static_cast<scalar>(k.value())/mouse_inverse_speed;
			break;
		default:
			break;
	}
}
