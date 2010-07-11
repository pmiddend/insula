#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/config/media_path.hpp>
#include <sge/renderer/refresh_rate_dont_care.hpp>
#include <sge/renderer/no_multi_sampling.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/system.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/var.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/filter/linear.hpp>
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
#include <boost/mpl/vector/vector10.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <cstdlib>
#include <exception>
#include <ostream>

int main(int,char *[])
try
{
	fcppt::log::activate_levels(
		sge::log::global(),
		fcppt::log::level::debug
	);

	sge::systems::instance sys(
		sge::systems::list()
		(
			sge::window::parameters(
				FCPPT_TEXT("insula toolbox, load image")
			)
		)
		(
			sge::renderer::parameters(
				sge::renderer::display_mode(
					sge::renderer::screen_size(
						1024,
						768
					),
					sge::renderer::bit_depth::depth32,
					sge::renderer::refresh_rate_dont_care
				),
				sge::renderer::depth_buffer::off,
				sge::renderer::stencil_buffer::off,
				sge::renderer::window_mode::windowed,
				sge::renderer::vsync::on,
				sge::renderer::no_multi_sampling
			)
		)
		(
			sge::systems::image_loader(
				sge::image::capabilities_field::null(),
				sge::all_extensions
			)
		)
	);
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
