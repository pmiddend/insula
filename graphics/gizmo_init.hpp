#ifndef INSULA_GRAPHICS_GIZMO_INIT_HPP_INCLUDED
#define INSULA_GRAPHICS_GIZMO_INIT_HPP_INCLUDED

#include "vec3.hpp"
#include <fcppt/math/vector/basic_impl.hpp>

namespace insula
{
namespace graphics
{
// NOTE: This is far from ideal. We have to force the user to specify
// _all_ 3 vectors and also, this class isn't much bigger than gizmo itself.
class gizmo_init
{
public:
	gizmo_init &
	forward(
		vec3 const &_forward)
	{
		forward_ = _forward;
		return *this;
	}

	gizmo_init &
	right(
		vec3 const &_right)
	{
		right_ = _right;
		return *this;
	}

	gizmo_init &
	up(
		vec3 const &_up)
	{
		up_ = _up;
		return *this;
	}

	vec3 const &right() const { return right_; }
	vec3 const &up() const { return up_; }
	vec3 const &forward() const { return forward_; }
private:
	vec3 right_,up_,forward_;
};
}
}

#endif
