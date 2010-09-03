#ifndef INSULA_MATH_BASIC_FRUSTUM_HPP_INCLUDED
#define INSULA_MATH_BASIC_FRUSTUM_HPP_INCLUDED

#include "plane/basic.hpp"
#include <array>

namespace insula
{
namespace math
{
template<typename T>
class basic_frustum
{
public:
	typedef
	plane::basic<T>
	plane;

	void left(plane const &v) { planes_[0] = v; }
	void right(plane const &v) { planes_[1] = v; }
	void top(plane const &v) { planes_[2] = v; }
	void bottom(plane const &v) { planes_[3] = v; }
	void near(plane const &v) { planes_[4] = v; }
	void far(plane const &v) { planes_[5] = v; }

	plane const &left() const { return planes_[0]; }
	plane const &right() const { return planes_[1]; }
	plane const &top() const { return planes_[2]; }
	plane const &bottom() const { return planes_[3]; }
	plane const &near() const { return planes_[4]; }
	plane const &far() const { return planes_[5]; }
private:
	typedef std::array<plane,6> array;

	array planes_;
};
}
}

#endif
