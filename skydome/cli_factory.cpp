#include "cli_factory.hpp"
#include "object.hpp"
#include "vec3_to_color.hpp"
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

insula::skydome::object_ptr const
insula::skydome::cli_factory(
	boost::program_options::variables_map const &vm,
	graphics::camera &camera,
	sge::renderer::device_ptr const renderer)
{
	return 
		fcppt::make_shared_ptr<object>(
			camera,
			renderer,
			get_option<graphics::scalar>(vm,"skydome-angle"),
			get_option<size_type>(vm,"skydome-latitudes"),
			get_option<size_type>(vm,"skydome-longitudes"),
			gradient(
				vec3_to_color(
					get_option<graphics::vec3>(vm,"skydome-color0")),
				vec3_to_color(
					get_option<graphics::vec3>(vm,"skydome-color1"))));
}
