#ifndef INSULA_TEXTURES_INTERPOLATORS_BASE_HPP_INCLUDED
#define INSULA_TEXTURES_INTERPOLATORS_BASE_HPP_INCLUDED

#include "../weights.hpp"

namespace insula
{
namespace textures
{
namespace interpolators
{
class base
{
public:
	base() = default;
	
	base &operator=(
		base const &) = delete;

	base(
		const base&) = delete;
	
	virtual weights const
	calculate_weights(
		weight place,
		weight gradient) = 0;

	virtual ~base() {};
};
}
}
}

#endif
