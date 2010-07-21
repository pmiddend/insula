#include "media_path.hpp"
#include "height_map/array.hpp"
#include "height_map/image_to_array.hpp"
#include "height_map/normalize_and_stretch.hpp"
#include "height_map/array_to_image.hpp"
#include "height_map/generate_gradient.hpp"
#include "textures/interpolators/bernstein_polynomial.hpp"
#include "textures/blend.hpp"
#include "textures/image_sequence.hpp"
#include "textures/rgb_view.hpp"
#include "textures/rgb_view_sequence.hpp"
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/config/media_path.hpp>
#include <sge/input/system.hpp>
#include <sge/input/action.hpp>
#include <sge/image/multi_loader.hpp>
#include <sge/image/capabilities.hpp>
#include <sge/image/loader.hpp>
#include <sge/image/file.hpp>
#include <sge/time/millisecond.hpp>
#include <sge/time/second.hpp>
#include <sge/time/default_callback.hpp>
#include <sge/mainloop/dispatch.hpp>
#include <sge/log/global.hpp>
#include <sge/all_extensions.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/log/level.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/assert.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <mizuiro/image/make_const_view.hpp>
#include <cstdlib>
#include <exception>
#include <ostream>
#include <iterator>
#include <algorithm>
#include <cmath>

int main(int const argc,char *argv[])
try
{
	fcppt::log::activate_levels(
		sge::log::global(),
		fcppt::log::level::debug);

	if (argc < 4)
	{
		fcppt::io::cerr 
			<< FCPPT_TEXT("usage: ") 
			<< fcppt::from_std_string(argv[0]) 
			<< FCPPT_TEXT("<height-map-file> <gradient-image-file> <image-file>*\n");
		return EXIT_FAILURE;
	}

	fcppt::filesystem::path const 
		heightmap_filename = 
			fcppt::from_std_string(
				argv[1]),
		gradient_image_filename = 
			fcppt::from_std_string(
				argv[2]);

	sge::systems::instance sys(
		sge::systems::list()
		(
			sge::systems::image_loader(
				sge::image::capabilities_field::null(),
				sge::all_extensions)));

	insula::height_map::array h = 
		insula::height_map::image_to_array(
			sys.image_loader().load(
				heightmap_filename));
	
	// Optional postprocessing
	/*
	std::transform(
		h.data(),
		h.data() + h.num_elements(),
		h.data(),
//		[](insula::height_map::array::element const s) { return s*s; });
		[](insula::height_map::array::element const s) { return std::sin(s); });
		*/

	insula::height_map::normalize_and_stretch(
		h);
	
	insula::height_map::array grad = 
		insula::height_map::generate_gradient(
			h);
	
	insula::height_map::normalize_and_stretch(
		grad);
	
	std::transform(
		grad.data(),
		grad.data() + h.num_elements(),
		grad.data(),
		[](insula::height_map::array::element const s) { return std::sin(s); });
	
	insula::height_map::array_to_image(
		grad,
		sys.image_loader(),
		insula::media_path()/FCPPT_TEXT("result_gradient.png"));
	
	sge::image::file_ptr const gradient_image = 
		sys.image_loader().load(
			gradient_image_filename);
	insula::textures::image_sequence images;
	
	std::transform(
		argv + 3,
		argv + argc,
		std::back_inserter<insula::textures::image_sequence>(
			images),
		[&sys](char const *c) { return sys.image_loader().load(fcppt::from_std_string(c)); });

	insula::textures::rgb_view const gradient_view = 
		gradient_image->view().get<insula::textures::rgb_view>();
	
	insula::textures::rgb_view_sequence views;

	std::transform(
		images.begin(),
		images.end(),
		std::back_inserter<insula::textures::rgb_view_sequence>(
			views),
		[](sge::image::file_ptr const f) { return f->view().get<insula::textures::rgb_view>(); });
	
	insula::textures::interpolators::bernstein_polynomial bp(
		views.size());
	
	insula::textures::rgb_store const result = 
		insula::textures::blend(
			gradient_view,
			views,
			h,
			grad,
			bp);

	sys.image_loader().loaders().at(0)->create(
		mizuiro::image::make_const_view(
			result.view()))->save(insula::media_path()/FCPPT_TEXT("result.png"));
	
	/*
	result->save(
		FCPPT_TEXT("media/result.png"))*/;
}
catch (fcppt::variant::invalid_get const &)
{
	throw fcppt::exception(FCPPT_TEXT("Tried to use an image as height texture which is not rgb8"));
}
catch(fcppt::exception const &e)
{
	fcppt::io::cerr << e.string() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}
catch(std::exception const &e)
{
	fcppt::io::cerr << e.what() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}
