#ifndef INSULA_GRAPHICS_BASIC_GIZMO_HPP_INCLUDED
#define INSULA_GRAPHICS_BASIC_GIZMO_HPP_INCLUDED

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

	// NOTE: One of the few places where explicit is not really needed
	basic_gizmo(
		init const &b)
	:
		forward_(b.forward()),
		right_(b.right()),
		up_(b.up())
	{

	}

	basic_gizmo &operator=(
		init const &b)
	{
		right_ = b.right();
		up_ = b.up();
		forward_ = b.forward();
		return *this;
	}

	T const &
	forward() const
	{
		return forward_;
	}

	T const &
	right() const
	{
		return right_;
	}

	T const &
	up() const
	{
		return up_;
	}
private:
	T forward_,right_,up_;
};
}
}

#endif
