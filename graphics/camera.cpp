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
#include <fcppt/math/vector/construct.hpp>
#include <fcppt/math/vector/cross.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/normalize.hpp>
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

insula::graphics::vec4 const
myconstruct(
	insula::graphics::vec3 const &v)
{
	return 
		fcppt::math::vector::construct(
			v,
			static_cast<insula::graphics::scalar>(
				0));
}
}

insula::graphics::camera::camera(
	console::object &_con,
	scalar _aspect,
	scalar _fov,
	scalar _near,
	scalar _far,
	scalar _speed,
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
	dirs_(
		vec3::null()),
	position_(
		_position),
	forward_(
		0,0,1,0),
	right_(
		1,0,0,0),
	up_(
		0,1,0,0),
	do_roll_(
		static_cast<scalar>(0)),
	roll_speed_(
		fcppt::math::pi<scalar>()/static_cast<scalar>(4))
{
}

void
insula::graphics::camera::update(
	scalar const t)
{
	if (!fcppt::math::almost_zero(do_roll_))
	{
		using fcppt::math::matrix::rotation_axis;
		using fcppt::math::vector::narrow_cast;

		up_ = 
			rotation_axis(
				-do_roll_ * roll_speed_ * t,
				narrow_cast<vec3>(forward_)) * up_;

		right_ = myconstruct(cross(narrow_cast<vec3>(up_),narrow_cast<vec3>(forward_)));
		forward_ = myconstruct(cross(narrow_cast<vec3>(right_),narrow_cast<vec3>(up_)));

		right_ = myconstruct(normalize(narrow_cast<vec3>(right_)));
		up_ = myconstruct(normalize(narrow_cast<vec3>(up_)));
		forward_ = myconstruct(normalize(narrow_cast<vec3>(forward_)));
	}

	position_ = 
		position_ + 
		speed_ * 
		t * 
		fcppt::math::vector::narrow_cast<vec3>(
			dirs_[0] * right_ + 
			dirs_[1] * up_ + 
			dirs_[2] * forward_);
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
	return 
		mat4(
			right_[0],right_[1],right_[2],static_cast<scalar>(0), 
			up_[0],up_[1],up_[2],static_cast<scalar>(0), 
			forward_[0],forward_[1],forward_[2],static_cast<scalar>(0), 
			static_cast<scalar>(0), static_cast<scalar>(0), static_cast<scalar>(0), static_cast<scalar>(1));
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

	scalar const angle = 
		static_cast<scalar>(k.value())/mouse_inverse_speed;

	switch (k.key().code())
	{
		case sge::input::kc::mouse_x_axis:
		{
			using fcppt::math::matrix::rotation_axis;
			using fcppt::math::vector::narrow_cast;

			forward_ = 
				rotation_axis(
					-angle,
					narrow_cast<vec3>(up_)) * forward_;

			right_ = myconstruct(cross(narrow_cast<vec3>(up_),narrow_cast<vec3>(forward_)));
			up_ = myconstruct(cross(narrow_cast<vec3>(forward_),narrow_cast<vec3>(right_)));

			right_ = myconstruct(normalize(narrow_cast<vec3>(right_)));
			up_ = myconstruct(normalize(narrow_cast<vec3>(up_)));
			forward_ = myconstruct(normalize(narrow_cast<vec3>(forward_)));
		}
		break;
		case sge::input::kc::mouse_y_axis:
		{
			using fcppt::math::matrix::rotation_axis;
			using fcppt::math::vector::narrow_cast;

			forward_ = 
				rotation_axis(
					-angle,
					narrow_cast<vec3>(right_)) * forward_;
			up_ = myconstruct(cross(narrow_cast<vec3>(forward_),narrow_cast<vec3>(right_)));
			right_ = myconstruct(cross(narrow_cast<vec3>(up_),narrow_cast<vec3>(forward_)));

			right_ = myconstruct(normalize(narrow_cast<vec3>(right_)));
			up_ = myconstruct(normalize(narrow_cast<vec3>(up_)));
			forward_ = myconstruct(normalize(narrow_cast<vec3>(forward_)));
		}
		break;
		case sge::input::kc::key_space:
			dirs_[1] = !k.value() ? 0 : -1;
			break;
		case sge::input::kc::key_lctrl:
			dirs_[1] = !k.value() ? 0 : 1;
			break;
		default:
			if (k.key().char_code() == FCPPT_TEXT('w'))
				dirs_[2] = !k.value() ? 0 : 1;

			if (k.key().char_code() == FCPPT_TEXT('s'))
				dirs_[2] = !k.value() ? 0 : -1;

			if (k.key().char_code() == FCPPT_TEXT('a'))
				dirs_[0] = !k.value() ? 0 : 1;

			if (k.key().char_code() == FCPPT_TEXT('d'))
				dirs_[0] = !k.value() ? 0 : -1;

			if (k.key().char_code() == FCPPT_TEXT('q'))
				do_roll_ = !k.value() ? 0 : -1;

			if (k.key().char_code() == FCPPT_TEXT('e'))
				do_roll_ = !k.value() ? 0 : 1;
			break;
	}
}
