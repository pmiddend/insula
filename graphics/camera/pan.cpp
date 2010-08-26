#include "pan.hpp"
#include "../../gizmo/orthogonalize_simple.hpp"
#include "../scalar.hpp"
#include "../../timed_output.hpp"
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/almost_zero.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/slerp.hpp>

insula::graphics::camera::pan::pan(
	gizmo const &_start,
	gizmo const &_end,
	scalar _speed)
:
	start_(
		_start),
	end_(
		_end),
	current_(
		start_),
	speed_(
		_speed)
{
}

void
insula::graphics::camera::pan::update(
	time_delta const t)
{
	current_.position(
		current_.position() + 
		static_cast<scalar>(t) * 
		(end_.position() - current_.position()) / 
		speed_);

	scalar const slerp_param = 
		length(current_.position() - end_.position())/
		length(start_.position() - end_.position());

	// We'd have to have a ternary transform which is too heavy, so an
	// explicit loop instead.
	for (gizmo::array_type::size_type i = 0; i < 3; ++i)
	{
		current_.array()[i] = 
			fcppt::math::vector::slerp(
				start_.array()[i],
				end_.array()[i],
				1.0f-slerp_param);
	}

	current_ = 
		insula::gizmo::orthogonalize_simple(
			current_);
}

bool
insula::graphics::camera::pan::finished() const
{
	//timed_output() << length(
	//		current_.position() - end_.position()) << "\n";
	return 
		// We cannot use almost_zero here since it's too strict
		length(
			current_.position() - end_.position()) < static_cast<graphics::scalar>(0.1);
}

insula::graphics::gizmo const
insula::graphics::camera::pan::current() const
{
	return current_;
}
