#include "height_map/array.hpp"
#include "height_map/image_to_array.hpp"
#include "display_2d_grid.hpp"
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/image/multi_loader.hpp>
#include <sge/image/capabilities.hpp>
#include <sge/log/global.hpp>
#include <sge/all_extensions.hpp>
#include <sge/exception.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/log/level.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/text.hpp>
#include <cstdlib>
#include <exception>
#include <ostream>

int main(int argc,char *argv[])
try
{
	if (argc != 2)
	{
		fcppt::io::cerr << FCPPT_TEXT("usage: ") << fcppt::from_std_string(argv[0]) << FCPPT_TEXT(" <image-file>\n");
		return EXIT_FAILURE;
	}

	fcppt::filesystem::path const filename(
		fcppt::from_std_string(
			argv[1]));

	fcppt::log::activate_levels(
		sge::log::global(),
		fcppt::log::level::debug
	);

	sge::systems::instance sys(
		sge::systems::list()
		(
			sge::systems::image_loader(
				sge::image::capabilities_field::null(),
				sge::all_extensions
			)
		)
	);

	insula::height_map::array const h = 
		insula::height_map::image_to_array(
			sys.image_loader().load(
				filename));
	
	fcppt::io::cout.precision(
		2);
	
	fcppt::io::cout.width(
		5);

	fcppt::io::cout.setf(
		std::ios::fixed);
	
	insula::display_2d_grid(
		fcppt::io::cout,
		h);
}
catch(sge::exception const &e)
{
	fcppt::io::cerr << e.string() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}
catch(std::exception const &e)
{
	fcppt::io::cerr << e.what() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}
