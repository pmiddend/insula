#ifndef INSULA_TEXTURES_INTERPOLATORS_BERNSTEIN_POLYNOMIAL_HPP_INCLUDED
#define INSULA_TEXTURES_INTERPOLATORS_BERNSTEIN_POLYNOMIAL_HPP_INCLUDED

#include <fcppt/tr1/array.hpp>
//#include <boost/math/special_functions/binomial.hpp>
#include <algorithm>
#include <cstddef>

namespace insula
{
namespace textures
{
namespace interpolators
{
template
<
	typename T,
	std::size_t degree
>
class bernstein_polynomial
{
public:
	typedef
	std::tr1::array
	<
		T,
		degree
	>
	coefficient_sequence;

	explicit
	bernstein_polynomial()
	{
	}

	std::tr1::array<T,degree> const
	weights(
		T const p,
		T const gradient)
	{
		
	}
private:
	coefficient_sequence c_;
};
}
}
}

#endif
