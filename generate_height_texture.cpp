#include "height_map/array.hpp"
#include "height_map/image_to_array.hpp"
#include "height_map/normalize_and_stretch.hpp"
#include "textures/interpolators/bernstein_polynomial.hpp"
#include "textures/blend.hpp"
#include "textures/image_sequence.hpp"
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
#include <vector>
#include <ostream>
#include <iterator>
#include <algorithm>
#include <cmath>

namespace
{
typedef
std::vector
<
	fcppt::filesystem::path
>
filename_sequence;
}

int main(int const argc,char *argv[])
try
{
	fcppt::log::activate_levels(
		sge::log::global(),
		fcppt::log::level::debug);

	if (argc < 3)
	{
		fcppt::io::cerr << FCPPT_TEXT("usage: ") << fcppt::from_std_string(argv[0]) << FCPPT_TEXT(" <image-file>\n");
		return EXIT_FAILURE;
	}

	fcppt::filesystem::path const heightmap_filename(
		fcppt::from_std_string(
			argv[1]));
	
	filename_sequence height_textures;
	
	for (int i = 2; i < argc; ++i)
		height_textures.push_back(
			argv[i]);
	
	FCPPT_ASSERT(
		!height_textures.empty());

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
	
	insula::textures::image_sequence images;
	
	std::transform(
		height_textures.begin(),
		height_textures.end(),
		std::back_inserter<insula::textures::image_sequence>(
			images),
		[&sys](fcppt::filesystem::path const &p) { return sys.image_loader().load(p); });
	
	fcppt::io::cout << FCPPT_TEXT("There are ") << images.size() << FCPPT_TEXT(" images\n");
	
	insula::textures::interpolators::bernstein_polynomial bp(
		images.size());
	
	insula::textures::rgb_store const result = 
		insula::textures::blend(
			images,
			h,
			bp);

	sys.image_loader().loaders().at(0)->create(
		mizuiro::image::make_const_view(
			result.view()))->save(FCPPT_TEXT("media/result.png"));
	
	/*
	result->save(
		FCPPT_TEXT("media/result.png"))*/;
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
