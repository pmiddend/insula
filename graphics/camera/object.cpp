#include "object.hpp"
#include "../../gizmo/rotation_to_mat4.hpp"
#include "../scalar.hpp"
#include "../vec4.hpp"
#include "../mat3.hpp"
#include "../../input_delegator.hpp"
#include <sge/input/key_pair.hpp>
#include <sge/input/system.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/matrix/perspective.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/matrix/rotation_axis.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/output.hpp>
#include <fcppt/math/matrix/transpose.hpp>
#include <fcppt/math/vector/narrow_cast.hpp>
#include <fcppt/math/vector/construct.hpp>
#include <fcppt/math/vector/cross.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/normalize.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/vector.hpp>
#include <fcppt/math/almost_zero.hpp>
#include <fcppt/math/compare.hpp>
#include <fcppt/math/pi.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/assert.hpp>

#include <functional>
#include <cmath>

insula::graphics::camera::object::object(
	input_delegator &input_delegator_,
	scalar _aspect,
	scalar _fov,
	scalar _near,
	scalar _far,
	scalar _speed,
	scalar _roll_speed,
	vec3 const &_position)
:
	input_connection_(
		input_delegator_.register_callback(
			std::bind(
				&object::input_callback,
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
	gizmo_(
		insula::graphics::gizmo::init()
			.position(
				_position)
			.forward(
				vec3(0,0,1))
			.right(
				vec3(1,0,0))
			.up(
				vec3(0,1,0))),
	do_roll_(
		static_cast<scalar>(0)),
	roll_speed_(
		_roll_speed)
{
}

void
insula::graphics::camera::object::update(
	scalar const t)
{
	if (!fcppt::math::almost_zero(do_roll_))
	{
		using fcppt::math::matrix::rotation_axis;
		using fcppt::math::vector::narrow_cast;
		using fcppt::math::vector::construct;

		vec3 
			up = 
				narrow_cast<vec3>(
					rotation_axis(
						-do_roll_ * roll_speed_ * t,
						gizmo_.forward()) *
					construct(
						gizmo_.up(),
						static_cast<scalar>(0))),
			right = 
				cross(up,gizmo_.forward()),
			forward = 
				cross(right,up); 

		gizmo_ = 
			insula::graphics::gizmo::init()
				.position(gizmo_.position())
				.forward(normalize(forward))
				.up(normalize(up))
				.right(normalize(right));
	}

	gizmo_.position( 
		gizmo_.position() + 
		speed_ * 
		t * 
		std::inner_product(
			gizmo_.array().begin(),
			gizmo_.array().end(),
			dirs_.data(),
			vec3::null()));
}

insula::graphics::mat4 const
insula::graphics::camera::object::world() const
{
	return 
		rotation() * 
		translation();
	
}

insula::graphics::mat4 const
insula::graphics::camera::object::rotation() const
{
	return 
		insula::gizmo::rotation_to_mat4(
			gizmo_);
}

insula::graphics::mat4 const
insula::graphics::camera::object::translation() const
{
	return 
		fcppt::math::matrix::translation(
			-gizmo_.position());
	
}

insula::graphics::mat4 const
insula::graphics::camera::object::perspective() const
{
	return 
		fcppt::math::matrix::perspective(
			aspect_,
			fov_,
			near_,
			far_);
}

insula::graphics::gizmo const &
insula::graphics::camera::object::gizmo() const
{
	return gizmo_;
}

insula::graphics::gizmo &
insula::graphics::camera::object::gizmo()
{
	return gizmo_;
}

insula::graphics::scalar
insula::graphics::camera::object::aspect() const
{
	return aspect_;
}

insula::graphics::scalar
insula::graphics::camera::object::fov() const
{
	return fov_;
}

void
insula::graphics::camera::object::input_callback(
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
		using fcppt::math::vector::construct;

		vec3 
			forward = 
				narrow_cast<vec3>(
					rotation_axis(
						-angle,
						gizmo_.up()) *
					construct(
						gizmo_.forward(),
						static_cast<scalar>(0))),
			right = 
				cross(gizmo_.up(),forward),
			up = 
				cross(forward,right); 

		gizmo_ = 
			gizmo::init()
				.position(gizmo_.position())
				.forward(normalize(forward))
				.up(normalize(up))
				.right(normalize(right));
		}
		break;
		case sge::input::kc::mouse_y_axis:
		{
			using fcppt::math::matrix::rotation_axis;
			using fcppt::math::vector::narrow_cast;
			using fcppt::math::vector::construct;

			vec3 
				forward = 
					narrow_cast<vec3>(
						rotation_axis(
							-angle,
							gizmo_.right()) *
						construct(
							gizmo_.forward(),
							static_cast<scalar>(0))),
				up = 
					cross(forward,gizmo_.right()),
				right = 
					cross(up,forward); 

			gizmo_ = 
				gizmo::init()
					.position(gizmo_.position())
					.forward(normalize(forward))
					.up(normalize(up))
					.right(normalize(right));
		}
		break;
		case sge::input::kc::key_space:
			dirs_[1] = !k.value() ? 0 : 1;
			break;
		case sge::input::kc::key_lctrl:
			dirs_[1] = !k.value() ? 0 : -1;
			break;
		default:
			if (k.key().code() == sge::input::kc::key_up)
				dirs_[2] = !k.value() ? 0 : -1;

			if (k.key().code() == sge::input::kc::key_down)
				dirs_[2] = !k.value() ? 0 : 1;

			if (k.key().code() == sge::input::kc::key_left)
				dirs_[0] = !k.value() ? 0 : -1;

			if (k.key().code() == sge::input::kc::key_right)
				dirs_[0] = !k.value() ? 0 : 1;

			if (k.key().char_code() == FCPPT_TEXT('q'))
				do_roll_ = !k.value() ? 0 : -1;

			if (k.key().char_code() == FCPPT_TEXT('e'))
				do_roll_ = !k.value() ? 0 : 1;
			break;
	}
}
