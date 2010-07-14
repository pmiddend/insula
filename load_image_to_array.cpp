#include "height_map.hpp"
#include "image_to_height_map.hpp"
#include "display_2d_array.hpp"
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/config/media_path.hpp>
#include <sge/input/system.hpp>
#include <sge/input/action.hpp>
#include <sge/image/multi_loader.hpp>
#include <sge/image/capabilities.hpp>
#include <sge/texture/manager.hpp>
#include <sge/texture/add_image.hpp>
#include <sge/texture/no_fragmented.hpp>
#include <sge/texture/default_creator.hpp>
#include <sge/texture/default_creator_impl.hpp>
#include <sge/time/millisecond.hpp>
#include <sge/time/second.hpp>
#include <sge/time/default_callback.hpp>
#include <sge/mainloop/dispatch.hpp>
#include <sge/log/global.hpp>
#include <sge/all_extensions.hpp>
#include <sge/exception.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/log/level.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/filesystem/is_regular.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
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

	insula::height_map const h = 
		insula::image_to_height_map(
			sys.image_loader().load(
				filename));
	
	fcppt::io::cout.precision(
		2);
	
	fcppt::io::cout.width(
		5);

	fcppt::io::cout.setf(
		std::ios::fixed);
	
	insula::display_2d_array(
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
