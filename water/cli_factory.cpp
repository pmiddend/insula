#include "cli_factory.hpp"
#include "object.hpp"
#include "../media_path.hpp"
#include "../get_option.hpp"
#include "../graphics/scalar.hpp"
#include "../graphics/rect.hpp"
#include "../graphics/vec3.hpp"
#include "../graphics/vec2.hpp"
#include "../graphics/dim2.hpp"
#include "../get_option.hpp"
#include <sge/image/multi_loader.hpp>
#include <fcppt/string.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/math/box/stretch.hpp>
#include <functional>

insula::water::object_ptr const
insula::water::cli_factory(
	boost::program_options::variables_map const &vm,
	sge::renderer::device_ptr const renderer,
	graphics::camera::object &camera,
	graphics::box const &extents,
	sge::image::multi_loader &image_loader,
	scene::manager &scene_manager)
{
	return 
		std::make_shared<object>(
			renderer,
			camera,
			get_option<graphics::scalar>(vm,"water-level"),
			fcppt::math::box::stretch(
				graphics::rect(
					graphics::vec2(
						extents.pos().x(),
						extents.pos().z()),
					graphics::dim2(
						extents.dimension().w(),
						extents.dimension().d())),
				get_option<graphics::scalar>(vm,"water-stretching")),
			get_option<sge::renderer::dim_type>(vm,"water-reflection-size"),
			image_loader.load(
				media_path()/get_option<fcppt::string>(vm,"water-bump-map")),
			get_option<graphics::scalar>(vm,"water-bump-scaling"),
			get_option<graphics::scalar>(vm,"water-wave-height"),
			get_option<graphics::scalar>(vm,"water-wind-speed"),
			scene_manager);
}
