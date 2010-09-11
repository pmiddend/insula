#ifndef INSULA_MATH_SAWTOOTH_HPP_INCLUDED
#define INSULA_MATH_SAWTOOTH_HPP_INCLUDED

#include <cmath>

namespace insula
{
namespace math
{
template<typename T>
T const
sawtooth(
	T const input,
	T const period)
{
	return 
		static_cast<T>(1) + 
		static_cast<T>(2) * (input/period - std::floor(input/period + static_cast<T>(0.5)));
}
}
}

#endif
