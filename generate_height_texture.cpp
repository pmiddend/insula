#include "media_path.hpp"
#include "stdlib/map.hpp"
#include "stdlib/normalize.hpp"
#include "height_map/array.hpp"
#include "height_map/image_to_array.hpp"
//#include "height_map/normalize_and_stretch.hpp"
#include "height_map/array_to_image.hpp"
#include "textures/interpolators/bernstein_polynomial.hpp"
#include "textures/blend.hpp"
#include "textures/image_sequence.hpp"
#include "textures/rgb_view.hpp"
#include "textures/rgb_view_sequence.hpp"
#include "stdlib/grid/sobel_operator.hpp"
#include "stdlib/grid/average_convolve.hpp"
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/config/media_path.hpp>
#include <sge/image/multi_loader.hpp>
#include <sge/image/capabilities.hpp>
#include <sge/image/loader.hpp>
#include <sge/image/file.hpp>
#include <sge/time/millisecond.hpp>
#include <sge/time/second.hpp>
#include <sge/time/default_callback.hpp>
#include <sge/mainloop/dispatch.hpp>
#include <sge/log/global.hpp>
#include <sge/extension_set.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/container/grid/object_impl.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/log/level.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/assert.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <boost/program_options.hpp>
#include <mizuiro/image/make_const_view.hpp>
#include <vector>
#include <exception>
#include <ostream>
#include <iterator>
#include <algorithm>
#include <cstdlib>
#include <cmath>

using namespace insula;

int main(int const argc,char *argv[])
try
{
	fcppt::log::activate_levels(
		sge::log::global(),
		fcppt::log::level::debug);

	typedef
	std::vector<fcppt::string>
	string_vector;

	boost::program_options::options_description desc("Allowed options");
	desc.add_options()
		("help","produce help message")
		("height-map",boost::program_options::value<fcppt::string>(),"Height map file (has to be grayscale)")
		("gradient-output",boost::program_options::value<fcppt::string>(),"Where to put the image gradient (path is not relative to media)")
		("image-output",boost::program_options::value<fcppt::string>(),"Where to put the blended image (again, not relative to media_path)")
		("image",boost::program_options::value<string_vector>()->multitoken(),"The image files")
		("gradient-image",boost::program_options::value<fcppt::string>(),"Texture to use for the gradient part of the blending process");

	boost::program_options::variables_map vm;

	boost::program_options::store(
		boost::program_options::parse_command_line(
			argc, 
			argv, 
			desc), 
			vm);

	boost::program_options::notify(
		vm);    

	if (vm.count("help")) 
	{
		fcppt::io::cout << desc << FCPPT_TEXT("\n");
		return EXIT_SUCCESS;
	}

	fcppt::filesystem::path const 
		heightmap_filename = 
			fcppt::from_std_string(
				get_option<fcppt::string>(vm,"height-map")),
		gradient_image_filename = 
			fcppt::from_std_string(
				get_option<fcppt::string>(vm,"gradient-image"));

	fcppt::io::cout << "Creating image loader\n";

	sge::systems::instance sys(
		sge::systems::list()
		(
			sge::systems::image_loader(
				sge::image::capabilities_field::null(),
				{FCPPT_TEXT("png")})));

	fcppt::io::cout << "Image loader created, loading height map\n";

	height_map::array heights = 
		height_map::image_to_array(
			sys.image_loader().load(
				heightmap_filename));

	fcppt::io::cout << "Height map loaded, normalizing\n";
	
	// Optional postprocessing
	/*
	std::transform(
		h.data(),
		h.data() + h.num_elements(),
		h.data(),
//		[](height_map::array::element const s) { return s*s; });
		[](height_map::array::element const s) { return std::sin(s); });
		*/

	heights = 
		stdlib::normalize(
			heights);
	//height_map::normalize_and_stretch(
		//heights);

	fcppt::io::cout << "Normalized, now calculating gradient\n";
	
	height_map::array grad = 
		stdlib::normalize(
			stdlib::grid::average_convolve(
				stdlib::grid::average_convolve(
					stdlib::normalize(
						stdlib::grid::sobel_operator(
							heights)))));

	fcppt::io::cout << "Gradient calculated, normalizing\n";
	
	//height_map::normalize_and_stretch(
	//	grad);

	fcppt::io::cout << "Normalized, now transforming\n";
	
	/*
	std::transform(
		grad.data(),
		grad.data() + heights.num_elements(),
		grad.data(),
		[](height_map::array::element const s) { return std::sin(s); });
*/

	fcppt::io::cout << "Transformed. Now loading gradient image\n";

	sge::image::file_ptr const gradient_image = 
		sys.image_loader().load(
			gradient_image_filename);

	fcppt::io::cout << "Loaded. Now loading the other images\n";

	textures::image_sequence images = 
		stdlib::map<textures::image_sequence>(
			get_option<string_vector>(
				vm,
				"image"),
			[&sys](fcppt::string const &s) 
			{
				return sys.image_loader().load(
					s);
			});


	textures::rgb_view const gradient_view = 
		gradient_image->view().get<textures::rgb_view>();
	
	textures::rgb_view_sequence views;

	fcppt::io::cout << "Loaded " <<  images.size() << " images. Now transforming those images to views\n";

	std::transform(
		images.begin(),
		images.end(),
		std::back_inserter<textures::rgb_view_sequence>(
			views),
		[](sge::image::file_ptr const f) 
		{ 
			return f->view().get<textures::rgb_view>(); 
		});
	fcppt::io::cout << "Transformed, now blending\n";
	
	textures::interpolators::bernstein_polynomial bp(
		views.size());
	
	textures::rgb_store const result = 
		textures::blend(
			gradient_view,
			views,
			heights,
			grad,
			bp);

	fcppt::io::cout << "Blended, now storing the result\n";

	sys.image_loader().loaders().at(0)->create(
		mizuiro::image::make_const_view(
			result.view()))->save(get_option<fcppt::string>(vm,"image-output"));

	height_map::array_to_image(
		grad,
		sys.image_loader(),
		get_option<fcppt::string>(vm,"gradient-output"));
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
