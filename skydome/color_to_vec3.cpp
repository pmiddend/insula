#include "color_to_vec3.hpp"
#include "../graphics/scalar.hpp"
#include <fcppt/math/vector/basic_impl.hpp>
#include <mizuiro/color/channel/red.hpp>
#include <mizuiro/color/channel/green.hpp>
#include <mizuiro/color/channel/blue.hpp>

insula::graphics::vec3 const
insula::skydome::color_to_vec3(
	color const &c)
{
	return 
		graphics::vec3(
			static_cast<graphics::scalar>(
				c.get<mizuiro::color::channel::red>()),
			static_cast<graphics::scalar>(
				c.get<mizuiro::color::channel::green>()),
			static_cast<graphics::scalar>(
				c.get<mizuiro::color::channel::blue>()));
}
