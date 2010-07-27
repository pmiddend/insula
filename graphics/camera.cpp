#include "camera.hpp"
#include "scalar.hpp"
#include "vec4.hpp"
#include "../console/object.hpp"
#include <sge/input/key_pair.hpp>
#include <sge/input/system.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/matrix/perspective.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/matrix/rotation_x.hpp>
#include <fcppt/math/matrix/rotation_y.hpp>
#include <fcppt/math/matrix/rotation_z.hpp>
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
	console::object &_con,
	scalar _aspect,
	scalar _fov,
	scalar _near,
	scalar _far,
	scalar _speed,
	scalar _roll_speed,
	vec3 const &_position)
:
	input_connection_(
		_con.register_callback(
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
	roll_speed_(
		_roll_speed),
	rotate_x_(
		static_cast<scalar>(
			0)),
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
	mat4 const r = 
		fcppt::math::matrix::transpose(
			rotation());
	
	position_ = 
		position_ + 
		speed_ * 
		t * 
		(
			dirs_.x() * 
			vec3(r[0][0],r[0][1],r[0][2]) +

			dirs_.y() * 
			vec3(r[1][0],r[1][1],r[1][2]) +

			dirs_.z() * 
			vec3(r[2][0],r[2][1],r[2][2])
		);

	rotate_z_ += 
		roll_speed_ * 
		t *
		do_rotate_z_;
}

insula::graphics::mat4 const
insula::graphics::camera::world() const
{
	return 
		rotation() * 
		translation();
	
}

insula::graphics::mat4 const
insula::graphics::camera::rotation() const
{
	mat4 const 
		rotate = 
			fcppt::math::matrix::rotation_x(
				rotate_x_) *
			fcppt::math::matrix::rotation_y(
				rotate_y_) * 
			fcppt::math::matrix::rotation_z(
				rotate_z_);

	return 
		rotate;
}

insula::graphics::mat4 const
insula::graphics::camera::translation() const
{
	mat4 const 
		translate =
				fcppt::math::matrix::translation(
					position_);

	return 
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
			rotate_x_ += 
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

			if (k.key().char_code() == FCPPT_TEXT('q'))
				do_rotate_z_ = !k.value() ? 0 : 1;

			if (k.key().char_code() == FCPPT_TEXT('e'))
				do_rotate_z_ = !k.value() ? 0 : -1;
			break;
	}
}
