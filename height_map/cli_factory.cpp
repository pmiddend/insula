#include "cli_factory.hpp"
#include "image_to_array.hpp"
#include "parameters.hpp"
#include "object.hpp"
#include "../create_path.hpp"
#include "../get_option.hpp"
#include "../graphics/scalar.hpp"
#include "../graphics/vec3.hpp"
#include "../get_option.hpp"
#include <sge/image/multi_loader.hpp>
#include <fcppt/string.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/container/grid/object_impl.hpp>
#include <fcppt/text.hpp>

insula::height_map::object_ptr const
insula::height_map::cli_factory(
	boost::program_options::variables_map const &vm,
	graphics::camera::object &camera,
	sge::renderer::device_ptr const renderer,
	sge::image::multi_loader &image_loader,
	scene::manager &scene_manager)
{
	return 
		std::make_shared<object>(
			height_map::parameters(
				camera,
				renderer,
				image_to_array(
					image_loader.load(
						create_path(
							get_option<fcppt::string>(vm,"terrain-height-map"),
							FCPPT_TEXT("heightfields")))),
				get_option<graphics::scalar>(vm,"terrain-cell-size"),
				get_option<graphics::scalar>(vm,"terrain-height-scaling"),
				get_option<graphics::vec3>(vm,"terrain-sun-direction"),
				get_option<graphics::scalar>(vm,"terrain-ambient-light"),
				get_option<graphics::scalar>(vm,"terrain-texture-scaling"),
				image_loader.load(
					create_path(
						get_option<fcppt::string>(vm,"terrain-gradient-texture"),
						FCPPT_TEXT("textures"))),
				image_loader.load(
					create_path(
						get_option<fcppt::string>(vm,"terrain-height-texture-1"),
						FCPPT_TEXT("textures"))),
				image_loader.load(
					create_path(
						get_option<fcppt::string>(vm,"terrain-height-texture-2"),
						FCPPT_TEXT("textures"))),
				scene_manager));
}
