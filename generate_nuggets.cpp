#include "generate_nuggets.hpp"
#include "random_flat_point.hpp"
#include <algorithm>
#include <random>
#include <functional>

insula::nugget_sequence const
insula::generate_nuggets(
	height_map::object const &terrain,
	graphics::scalar const water_level,
	nugget_sequence::size_type const count)
{
	nugget_sequence result(
		count);

	std::mt19937 engine;

	std::generate(
		result.begin(),
		result.end(),
		std::bind(
			&random_flat_point,
			// This is a case for cref because terrain is incomplete
			std::cref(
				terrain),
			water_level,
			std::ref(
				engine)));

	return result;
}
