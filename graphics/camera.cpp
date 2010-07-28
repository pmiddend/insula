#include "camera.hpp"
#include "scalar.hpp"
#include "vec4.hpp"
#include "mat3.hpp"
#include "../console/object.hpp"
#include <sge/input/key_pair.hpp>
#include <sge/input/system.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/matrix/perspective.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/matrix/rotation_x.hpp>
#include <fcppt/math/matrix/rotation_y.hpp>
#include <fcppt/math/matrix/rotation_z.hpp>
#include <fcppt/math/matrix/rotation_axis.hpp>
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
#include <cmath>

namespace
{
template<typename T>
bool
mycompare(
	T const &a,
	T const &b)
{
	return std::abs(a-b) < static_cast<T>(0.0001);
}
}

insula::graphics::camera::camera(
	console::object &_con,
	scalar _aspect,
	scalar _fov,
	scalar _near,
	scalar _far,
	scalar _speed,
	//scalar _roll_speed,
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
			100);

	switch (k.key().code())
	{
		case sge::input::kc::mouse_x_axis:
		{
			break;
			scalar const rotate_amount = 
				static_cast<scalar>(k.value())/
				mouse_inverse_speed;

			/*
			fcppt::io::cout
				<< "IN("<< rotate_x_ << "," << rotate_y_ << "," << rotate_z_ << ")\n";*/

			vec4 const 
				e2 = 
					rotation() * vec4(0,1,0,1);

			mat4 const r = 
				fcppt::math::matrix::rotation_axis(
					rotate_amount,
					fcppt::math::vector::narrow_cast<vec3>(
						e2)) *
				rotation();
			
			vec4
				e1 = 
					r * vec4(1,0,0,1),
				e3 = 
					r * vec4(0,0,1,1);

			 graphics::scalar dummy_x,dummy_y,dummy_z;

			 fcppt::math::matrix::angles_from_matrix(
			 	mat3(
					e1[0],e2[0],e3[0],
					e1[1],e2[1],e3[1],
					e1[2],e2[2],e3[2]),
				rotate_x_,
				rotate_y_,
				rotate_z_,
				dummy_x,
				dummy_y,
				dummy_z,
				&mycompare<graphics::scalar>);

			/*
			fcppt::io::cout 
				<< "OUT(" << rotate_x_ << "," << rotate_y_ << "," << rotate_z_ << ")\n"
				<< "OUT(" << dummy_x << "," << dummy_y << "," << dummy_z << ")\n";*/
	//			<< "rotate y after: " 
	//			<< rotate_y_ << "\n";
		}
			break;
		case sge::input::kc::mouse_y_axis:
		{
			scalar const rotate_amount = 
				static_cast<scalar>(k.value())/
				mouse_inverse_speed;

			/*
			fcppt::io::cout
				<< "IN("<< rotate_x_ << "," << rotate_y_ << "," << rotate_z_ << ")\n";*/

			vec4 const 
				e1 = 
					rotation() * vec4(1,0,0,0);

			mat4 const r = 
				fcppt::math::matrix::rotation_axis(
					rotate_amount,
					fcppt::math::vector::narrow_cast<vec3>(
						e1)) *
				rotation();
			
			vec4
				e2 = 
					r * vec4(0,1,0,0),
				e3 = 
					r * vec4(0,0,1,0);

			 graphics::scalar dummy_x,dummy_y,dummy_z;

			 fcppt::math::matrix::angles_from_matrix(
			 	mat3(
					e1[0],e2[0],e3[0],
					e1[1],e2[1],e3[1],
					e1[2],e2[2],e3[2]),
				rotate_x_,
				rotate_y_,
				rotate_z_,
				dummy_x,
				dummy_y,
				dummy_z,
				&mycompare<graphics::scalar>);

			/*
			fcppt::io::cout 
				<< "OUT(" << rotate_x_ << "," << rotate_y_ << "," << rotate_z_ << ")\n"
				<< "OUT(" << dummy_x << "," << dummy_y << "," << dummy_z << ")\n";*/
		}
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
				rotate_z_ = !k.value() ? 0 : -1;
			break;
	}
}
