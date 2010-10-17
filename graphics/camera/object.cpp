#include "object.hpp"
#include "../../gizmo/rotation_to_mat4.hpp"
#include "../scalar.hpp"
#include "../vec4.hpp"
#include "../mat3.hpp"
#include "../../input_delegator.hpp"
#include "parameters.hpp"
#include <sge/input/mouse/axis_event.hpp>
#include <sge/input/mouse/axis.hpp>
#include <sge/input/keyboard/key_event.hpp>
#include <fcppt/math/matrix/perspective.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/matrix/rotation_axis.hpp>
#include <fcppt/math/matrix/transpose.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/assert.hpp>

#include <functional>
#include <cmath>

insula::graphics::camera::object::object(
	parameters const &params)
:
	key_callback_(
		params.input_delegator.key_callback(
			std::bind(
				&object::key_callback,
				this,
				std::placeholders::_1))),
	mouse_axis_callback_(
		params.input_delegator.mouse_axis_callback(
			std::bind(
				&object::mouse_axis_callback,
				this,
				std::placeholders::_1))),
	aspect_(
		params.aspect),
	fov_(
		params.fov),
	near_(
		params.near),
	far_(
		params.far),
	movement_speed_(
		params.movement_speed),
	rotation_speed_(
		params.rotation_speed),
	dirs_(
		vec3::null()),
	gizmo_(
		insula::graphics::gizmo::init()
			.position(
				params.position)
			.forward(
				vec3(0,0,1))
			.right(
				vec3(1,0,0))
			.up(
				vec3(0,1,0))),
	movement_(true)
{
}

void
insula::graphics::camera::object::update(
	scalar const t)
{
	if (movement_)
		gizmo_.position( 
			gizmo_.position() + 
			movement_speed_ * 
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

insula::graphics::mat4 const
insula::graphics::camera::object::mvp() const
{
	return 
		perspective() * world();
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
insula::graphics::camera::object::near() const
{
	return near_;
}

insula::graphics::scalar
insula::graphics::camera::object::far() const
{
	return far_;
}

insula::graphics::scalar
insula::graphics::camera::object::fov() const
{
	return fov_;
}

void
insula::graphics::camera::object::movement(
	bool const _movement)
{
	movement_ = _movement;
}

void
insula::graphics::camera::object::key_callback(
	sge::input::keyboard::key_event const &k)
{
	switch (k.key().code())
	{
		case sge::input::keyboard::key_code::space:
			dirs_[1] = !k.pressed() ? 0 : 1;
			break;
		case sge::input::keyboard::key_code::lctrl:
			dirs_[1] = !k.pressed() ? 0 : -1;
			break;
		case sge::input::keyboard::key_code::up:
			dirs_[2] = !k.pressed() ? 0 : -1;
			break;
		case sge::input::keyboard::key_code::down:
			dirs_[2] = !k.pressed() ? 0 : 1;
			break;
		case sge::input::keyboard::key_code::left:
			dirs_[0] = !k.pressed() ? 0 : -1;
			break;
		case sge::input::keyboard::key_code::right:
			dirs_[0] = !k.pressed() ? 0 : 1;
			break;
		default:
			break;
	}
}

void
insula::graphics::camera::object::mouse_axis_callback(
	sge::input::mouse::axis_event const &k)
{
	scalar const angle = 
		static_cast<scalar>(
			k.axis_position())
			/rotation_speed_;

	switch (k.axis())
	{
		case sge::input::mouse::axis::x:
		{
		using fcppt::math::matrix::rotation_axis;
		using fcppt::math::vector::narrow_cast;
		using fcppt::math::vector::construct;
		using fcppt::math::vector::normalize;

		vec3 
			forward = 
				narrow_cast<vec3>(
					rotation_axis(
						-angle,
						vec3(0,1,0)) *
					construct(
						gizmo_.forward(),
						// Cast neccesary here
						static_cast<scalar>(0))),
			right = 
				cross(
					vec3(0,1,0),
					forward),
			up = 
				cross(
					forward,
					right); 

		gizmo_ = 
			gizmo::init()
				.position(gizmo_.position())
				.forward(normalize(forward))
				.up(normalize(up))
				.right(normalize(right));
		}
		break;
		case sge::input::mouse::axis::y:
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
		case sge::input::mouse::axis::wheel:
		break;
	}
}
