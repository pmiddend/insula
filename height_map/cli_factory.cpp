#include "cli_factory.hpp"
#include "image_to_array.hpp"
#include "object.hpp"
#include "../media_path.hpp"
#include "../get_option.hpp"
#include "../graphics/scalar.hpp"
#include "../graphics/vec3.hpp"
#include "../get_option.hpp"
#include <sge/image/multi_loader.hpp>
#include <fcppt/string.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/assert_message.hpp>
#include <fcppt/text.hpp>

insula::height_map::object_ptr const
insula::height_map::cli_factory(
	boost::program_options::variables_map const &vm,
	graphics::camera &camera,
	sge::renderer::device_ptr const renderer,
	sge::image::multi_loader &image_loader)
{
	std::vector<fcppt::string> const height_textures = 
		vm["height-texture"].as<std::vector<fcppt::string>>();

	FCPPT_ASSERT_MESSAGE(
		height_textures.size() == 2,
		FCPPT_TEXT("There must be exactly two height textures"));

	return 
		fcppt::make_shared_ptr<object>(
			camera,
			renderer,
			image_to_array(
				image_loader.load(
					media_path()/get_option<fcppt::string>(vm,"height-map"))),
			get_option<graphics::scalar>(vm,"cell-size"),
	//		get_option<graphics::scalar>(vm,"cell-size"),
			get_option<graphics::scalar>(vm,"height-scaling"),
			get_option<graphics::vec3>(vm,"sun-direction"),
			get_option<graphics::scalar>(vm,"ambient-light"),
			get_option<graphics::scalar>(vm,"texture-scaling"),
			image_loader.load(
				media_path()/get_option<fcppt::string>(vm,"gradient-texture")),
			image_loader.load(
				media_path()/height_textures[0]),
			image_loader.load(
				media_path()/height_textures[1]));
}
