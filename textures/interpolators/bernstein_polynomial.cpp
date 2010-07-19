#include "bernstein_polynomial.hpp"
#include <boost/math/special_functions/binomial.hpp>
#include <fcppt/io/cout.hpp>
#include <algorithm>
#include <cmath>

insula::textures::interpolators::bernstein_polynomial::bernstein_polynomial(
	weights::size_type const _count)
:
	count_(
		_count)
{
}

insula::textures::weight
insula::textures::interpolators::bernstein_polynomial::at(
	unsigned i,
	weight const t)
{
	return 
		boost::math::binomial_coefficient<weight>(
			static_cast<unsigned>(
				count_-1),
			i) *
		std::pow(
			t,
			static_cast<weight>(
				i)) *
		std::pow(
			static_cast<weight>(1)-t,
			static_cast<weight>(count_-i-1));
}

insula::textures::weights const
insula::textures::interpolators::bernstein_polynomial::calculate_weights(
	weight const p,
	weight const gradient)
{
	weights::weight_sequence w(
		count_);
	for (weights::size_type i = 0; i < count_; ++i)
		w.push_back(
			at(
				static_cast<unsigned>(
					i),
				p));
	return 
		weights(
			w,
			gradient);
}
