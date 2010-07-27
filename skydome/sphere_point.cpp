#include "sphere_point.hpp"
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <cmath>

insula::skydome::vf::packed_position
insula::skydome::sphere_point(
	graphics::scalar radius,
	graphics::scalar lat,
	graphics::scalar lon)
{
	using std::sin;
	using std::cos;

	return 
		radius * 
		insula::skydome::vf::packed_position(
			cos(lon) * sin(lat),
			cos(lat),
			sin(lon) * sin(lat));
}
