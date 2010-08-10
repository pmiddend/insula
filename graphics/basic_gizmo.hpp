#ifndef INSULA_GRAPHICS_BASIC_GIZMO_HPP_INCLUDED
#define INSULA_GRAPHICS_BASIC_GIZMO_HPP_INCLUDED

#include <array>

namespace insula
{
namespace graphics
{
template<typename T>
class basic_gizmo
{
public:
	typedef T value_type;

	// Ugly inner class, but it's pretty good here, I think
	class init
	{
	public:
		init &
		forward(
			T const &_forward)
		{
			forward_ = _forward;
			return *this;
		}

		init &
		right(
			T const &_right)
		{
			right_ = _right;
			return *this;
		}

		init &
		up(
			T const &_up)
		{
			up_ = _up;
			return *this;
		}

		T const &right() const { return right_; }
		T const &up() const { return up_; }
		T const &forward() const { return forward_; }
	private:
		T right_,up_,forward_;
	};

	typedef
	std::array<T,3>
	array_type;

	// NOTE: One of the few places where explicit is not really needed
	basic_gizmo(
		init const &b)
	{
		array_[0] = b.right();
		array_[1] = b.up();
		array_[2] = b.forward();
	}

	basic_gizmo &operator=(
		init const &b)
	{
		array_[0] = b.right();
		array_[1] = b.up();
		array_[2] = b.forward();
		return *this;
	}

	T const &
	right() const
	{
		return array_[0];
	}

	T const &
	up() const
	{
		return array_[1];
	}

	T const &
	forward() const
	{
		return array_[2];
	}

	array_type const &
	array() const
	{
		return array_;
	}
private:
	array_type array_;
};
}
}

#endif
