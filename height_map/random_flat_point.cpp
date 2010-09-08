#include "random_flat_point.hpp"
#include "object.hpp"

insula::height_map::vec2 const
insula::height_map::random_flat_point(
	object const &o,
	scalar /*water_level*/,
	random_engine &engine)
{
	// For now, generate just some random point
	std::uniform_real_distribution<height_map::scalar> 
		xdist(
			o.extents().left(),
			o.extents().right()),
		ydist(
			o.extents().front(),
			o.extents().back());

	return 
		vec2(
			xdist(
				engine),
			ydist(
				engine));
}
