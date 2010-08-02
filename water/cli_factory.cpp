#include "cli_factory.hpp"
#include "object.hpp"
#include "../media_path.hpp"
#include "../get_option.hpp"
#include "../graphics/scalar.hpp"
#include "../get_option.hpp"
#include <sge/image/multi_loader.hpp>
#include <fcppt/string.hpp>
#include <fcppt/make_shared_ptr.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <functional>

insula::water::object_ptr const
insula::water::cli_factory(
	boost::program_options::variables_map const &vm,
	sge::renderer::device_ptr const renderer,
	graphics::camera &camera,
	graphics::rect const &extents,
	sge::image::multi_loader &image_loader)
{
	return 
		fcppt::make_shared_ptr<object>(
			renderer,
			std::ref(
				camera),
			get_option<graphics::scalar>(vm,"water-level"),
			extents,
			get_option<sge::renderer::dim_type>(vm,"water-reflection-size"),
			image_loader.load(
				media_path()/get_option<fcppt::string>(vm,"water-bump-map")));
}
