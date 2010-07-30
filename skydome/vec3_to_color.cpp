#include "vec3_to_color.hpp"
#include <fcppt/math/vector/basic_impl.hpp>
#include <mizuiro/color/init.hpp>

insula::skydome::color const
insula::skydome::vec3_to_color(
	graphics::vec3 const &v)
{
	return 
		color(
			(mizuiro::color::init::red = v[0])
			(mizuiro::color::init::green = v[1])
			(mizuiro::color::init::blue = v[2]));
}
