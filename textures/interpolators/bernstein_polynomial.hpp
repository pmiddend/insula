#ifndef INSULA_TEXTURES_INTERPOLATORS_BERNSTEIN_POLYNOMIAL_HPP_INCLUDED
#define INSULA_TEXTURES_INTERPOLATORS_BERNSTEIN_POLYNOMIAL_HPP_INCLUDED

#include "base.hpp"
#include "../weights.hpp"

namespace insula
{
namespace textures
{
namespace interpolators
{
class bernstein_polynomial
:
	public base
{
public:
	explicit
	bernstein_polynomial(
		weights::size_type);

	weight
	at(
		unsigned,
		weight);

	weights const
	calculate_weights(
		weight,
		weight);
private:
	weights::size_type count_;
};
}
}
}

#endif
