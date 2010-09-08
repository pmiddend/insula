#include "generate_nuggets.hpp"
#include "height_map/random_flat_point.hpp"
#include "random_engine.hpp"
#include "random_seed.hpp"
#include <algorithm>
#include <functional>

insula::nugget_sequence const
insula::generate_nuggets(
	height_map::object const &terrain,
	graphics::scalar const water_level,
	nugget_sequence::size_type const count)
{
	nugget_sequence result(
		count);

	random_engine engine(
		random_seed());

	std::generate(
		result.begin(),
		result.end(),
		std::bind(
			&height_map::random_flat_point,
			// This is a case for cref because terrain is incomplete
			std::cref(
				terrain),
			water_level,
			std::ref(
				engine)));

	return result;
}
