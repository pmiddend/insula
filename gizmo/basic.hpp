#ifndef INSULA_GIZMO_BASIC_HPP_INCLUDED
#define INSULA_GIZMO_BASIC_HPP_INCLUDED

#include "size_type.hpp"
#include <fcppt/math/vector/vector.hpp>
#include <array>

namespace insula
{
namespace gizmo
{
template<typename Scalar,size_type N>
class basic
{
public:
	typedef 
	Scalar 
	scalar;

	typedef typename
	fcppt::math::vector::static_<scalar,N>::type
	vector;

	typedef
	std::array<vector,3>
	array_type;

	// Ugly inner class, but it's pretty good here, I think
	class init
	{
	public:
		init &
		position(vector const &_position)
		{
			position_ = _position;
			return *this;
		}
		
		init &
		forward(
			vector const &_forward)
		{
			forward_ = _forward;
			return *this;
		}

		init &
		right(
			vector const &_right)
		{
			right_ = _right;
			return *this;
		}

		init &
		up(
			vector const &_up)
		{
			up_ = _up;
			return *this;
		}

		vector const &position() const { return position_; }
		vector const &right() const { return right_; }
		vector const &up() const { return up_; }
		vector const &forward() const { return forward_; }
	private:
		vector position_;
		vector right_,up_,forward_;
	};

	// NOTE: One of the few places where explicit is not really needed
	basic(
		init const &b)
	:
		position_(
			b.position())
	{
		array_[0] = b.right();
		array_[1] = b.up();
		array_[2] = b.forward();
	}

	basic &operator=(
		init const &b)
	{
		position_ = b.position();
		array_[0] = b.right();
		array_[1] = b.up();
		array_[2] = b.forward();
		return *this;
	}

	vector const &
	position() const
	{
		return position_;
	}

	vector const &
	right() const
	{
		return array_[0];
	}

	vector const &
	up() const
	{
		return array_[1];
	}

	vector const &
	forward() const
	{
		return array_[2];
	}

	array_type const &
	array() const
	{
		return array_;
	}

	array_type &
	array()
	{
		return array_;
	}

	void
	position(
		vector const &p)
	{
		position_ = p;
	}

	void
	right(
		vector const &p)
	{
		array_[0] = p;
	}

	void
	up(
		vector const &p)
	{
		array_[1] = p;
	}

	void
	forward(
		vector const &p)
	{
		array_[2] = p;
	}
private:
	vector position_;
	array_type array_;
};
}
}

#endif
