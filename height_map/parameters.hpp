#ifndef INSULA_HEIGHT_MAP_PARAMETERS_HPP_INCLUDED
#define INSULA_HEIGHT_MAP_PARAMETERS_HPP_INCLUDED

#include "../graphics/camera/object_fwd.hpp"
#include "../graphics/scalar.hpp"
#include "../graphics/vec3.hpp"
#include "array.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/image/file_ptr.hpp>
#include <fcppt/math/vector/basic_impl.hpp>

namespace insula
{
namespace height_map
{
class parameters
{
public:
	graphics::camera::object const &camera;
	sge::renderer::device_ptr renderer;
	height_map::array array;
	graphics::scalar cell_size;
	graphics::scalar height_scaling;
	graphics::vec3 sun_direction;
	graphics::scalar ambient_light;
	graphics::scalar texture_scaling;
	sge::image::file_ptr gradient_texture_image;
	sge::image::file_ptr lower_texture_image;
	sge::image::file_ptr upper_texture_image;

	explicit
	parameters(
		graphics::camera::object const &camera,
		sge::renderer::device_ptr renderer,
		height_map::array const &array,
		graphics::scalar const &cell_size,
		graphics::scalar const height_scaling,
		graphics::vec3 const &sun_direction,
		graphics::scalar const ambient_light,
		graphics::scalar const texture_scaling,
		sge::image::file_ptr const &gradient_texture_image,
		sge::image::file_ptr const &lower_texture_image,
		sge::image::file_ptr const &upper_texture_image)
	:
		camera(camera),
		renderer(renderer),
		array(array),
		cell_size(cell_size),
		height_scaling(height_scaling),
		sun_direction(sun_direction),
		ambient_light(ambient_light),
		texture_scaling(texture_scaling),
		gradient_texture_image(gradient_texture_image),
		lower_texture_image(lower_texture_image),
		upper_texture_image(upper_texture_image)
	{
	}
};
}
}

#endif
