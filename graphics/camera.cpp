#include "camera.hpp"
#include "scalar.hpp"
#include "vec4.hpp"
#include <sge/input/key_pair.hpp>
#include <sge/input/system.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/matrix/perspective.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/matrix/rotation_x.hpp>
#include <fcppt/math/matrix/rotation_y.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/output.hpp>
#include <fcppt/math/matrix/transpose.hpp>
#include <fcppt/math/vector/narrow_cast.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/vector.hpp>
#include <fcppt/math/pi.hpp>
#include <fcppt/io/cout.hpp>

#include <functional>

insula::graphics::camera::camera(
	sge::input::system_ptr const _is,
	scalar _aspect,
	scalar _fov,
	scalar _near,
	scalar _far,
	scalar _speed,
	vec3 const &_position)
:
	input_connection_(
		_is->register_callback(
			std::bind(
				&camera::input_callback,
				this,
				std::placeholders::_1))),
	aspect_(
		_aspect),
	fov_(
		_fov),
	near_(
		_near),
	far_(
		_far),
	speed_(
		_speed),
	rotate_x_(
		fcppt::math::pi<scalar>()),
	rotate_y_(
		static_cast<scalar>(
			0)),
	dirs_(
		vec3::null()),
	position_(
		_position)
{
}

void
insula::graphics::camera::update(
	scalar const t)
{
	mat4 const rotation = 
		fcppt::math::matrix::rotation_x(
			rotate_x_) *
		fcppt::math::matrix::rotation_y(
			rotate_y_);
	
	position_ = 
		position_ + 
		speed_ * 
		t * 
		(
			dirs_.x() * 
			vec3(rotation[0][0],rotation[0][1],rotation[0][2]) +

			dirs_.y() * 
			vec3(rotation[1][0],rotation[1][1],rotation[1][2]) +

			dirs_.z() * 
			vec3(rotation[2][0],rotation[2][1],rotation[2][2])
		);
}

insula::graphics::mat4 const
insula::graphics::camera::world() const
{
	mat4 const 
		rotate = 
			fcppt::math::matrix::rotation_x(
				rotate_x_) *
			fcppt::math::matrix::rotation_y(
				rotate_y_),
		translate =
			fcppt::math::matrix::transpose(
				fcppt::math::matrix::translation(
					position_));

	return 
		rotate * 
		translate;
	
}

insula::graphics::mat4 const
insula::graphics::camera::perspective() const
{
	return 
		fcppt::math::matrix::perspective(
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
			rotate_x_ -= 
				static_cast<scalar>(k.value())/mouse_inverse_speed;
			break;
		case sge::input::kc::key_space:
			dirs_.y() = !k.value() ? 0 : -1;
			break;
		case sge::input::kc::key_lctrl:
			dirs_.y() = !k.value() ? 0 : 1;
			break;
		default:
			if (k.key().char_code() == FCPPT_TEXT('w'))
				dirs_.z() = !k.value() ? 0 : 1;

			if (k.key().char_code() == FCPPT_TEXT('s'))
				dirs_.z() = !k.value() ? 0 : -1;

			if (k.key().char_code() == FCPPT_TEXT('a'))
				dirs_.x() = !k.value() ? 0 : 1;

			if (k.key().char_code() == FCPPT_TEXT('d'))
				dirs_.x() = !k.value() ? 0 : -1;
			break;
	}
}
