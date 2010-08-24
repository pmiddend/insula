#include "random_flat_point.hpp"
#include "height_map/object.hpp"

insula::graphics::vec2 const
insula::random_flat_point(
	height_map::object const &o,
	graphics::scalar /*water_level*/,
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
		graphics::vec2(
			xdist(
				engine),
			ydist(
				engine));
}
