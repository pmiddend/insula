#include "graphics/scalar.hpp"
#include "graphics/vec2.hpp"
#include "graphics/vec3.hpp"
#include "graphics/camera.hpp"
#include "skydome/object.hpp"
#include "console/object.hpp"
#include <sge/log/global.hpp>
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/systems/parameterless.hpp>
#include <sge/systems/image_loader.hpp>
#include <sge/console/arg_list.hpp>
#include <sge/console/object.hpp>
#include <sge/image/capabilities_field.hpp>
#include <sge/window/parameters.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/parameters.hpp>
#include <sge/renderer/screen_size.hpp>
#include <sge/renderer/display_mode.hpp>
#include <sge/renderer/bit_depth.hpp>
#include <sge/renderer/refresh_rate_dont_care.hpp>
#include <sge/renderer/depth_buffer.hpp>
#include <sge/renderer/stencil_buffer.hpp>
#include <sge/renderer/window_mode.hpp>
#include <sge/renderer/vsync.hpp>
#include <sge/renderer/no_multi_sampling.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/state/draw_mode.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/color.hpp>
#include <sge/renderer/state/float.hpp>
#include <sge/mainloop/dispatch.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/exception.hpp>
#include <sge/image/colors.hpp>
#include <sge/time/timer.hpp>
#include <sge/time/second.hpp>
#include <sge/input/system.hpp>
#include <sge/input/action.hpp>
#include <sge/input/key_code.hpp>
#include <sge/image/multi_loader.hpp>
#include <sge/all_extensions.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/log/level.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/math/vector/input.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/string.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/text.hpp>
#include <fcppt/lexical_cast.hpp>
#include <fcppt/bad_lexical_cast.hpp>
#include <fcppt/filesystem/is_regular.hpp>
#include <fcppt/io/ifstream.hpp>
#include <boost/program_options.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <vector>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <ostream>

int main(int argc,char *argv[])
try
{
	using namespace insula;

	fcppt::log::activate_levels(
		sge::log::global(),
		fcppt::log::level::debug
	);

	// We need this to read the filename vector for the height textures
	typedef
	std::vector<fcppt::string>
	string_vector;

	boost::program_options::options_description desc("Allowed options");
	
	string_vector height_textures;

	desc.add_options()
		("help","produce help message")
		("fov",boost::program_options::value<graphics::scalar>()->default_value(90),"Field of view (in degrees)")
		("near",boost::program_options::value<graphics::scalar>()->default_value(1),"Distance to the near plane")
		("far",boost::program_options::value<graphics::scalar>()->default_value(10000),"Distance to the far plane")
		("latitudes",boost::program_options::value<skydome::size_type>()->default_value(4),"How many latitude iterations")
		("longitudes",boost::program_options::value<skydome::size_type>()->default_value(10),"How many longitude iterations")
		("angle",boost::program_options::value<graphics::scalar>()->default_value(static_cast<graphics::scalar>(90)),"Total angle (in degrees)");
	
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

	sge::systems::instance sys(
		sge::systems::list() 
		(
			sge::window::parameters(
				FCPPT_TEXT("render sky dome")))
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
				sge::renderer::depth_buffer::d24,
				sge::renderer::stencil_buffer::off,
				sge::renderer::window_mode::windowed,
				sge::renderer::vsync::on,
				sge::renderer::no_multi_sampling))
		(sge::systems::parameterless::input)
		(sge::systems::parameterless::font)
		(
			sge::systems::image_loader(
				sge::image::capabilities_field::null(),
				sge::all_extensions)));

	console::object console(
		sys.input_system(),
		sys.renderer(),
		sys.font_system(),
		sys.image_loader());
	
	graphics::camera cam(
		console,
		static_cast<graphics::scalar>(
			1024.0/768.0),
		static_cast<graphics::scalar>(
			fcppt::math::deg_to_rad(
				static_cast<graphics::scalar>(
					vm["fov"].as<graphics::scalar>()))),
		vm["near"].as<graphics::scalar>(),
		vm["far"].as<graphics::scalar>(),
		graphics::scalar(0.5),
		graphics::vec3::null());

	skydome::object s(
		cam,
		sys.renderer(),
		console.model(),
		vm["angle"].as<graphics::scalar>(),
		vm["latitudes"].as<skydome::size_type>(),
		vm["longitudes"].as<skydome::size_type>());
	
	fcppt::signal::scoped_connection regenerate_conn(
		console.model().insert(
			FCPPT_TEXT("regenerate"),
			[&s,&sys](sge::console::arg_list const &args,sge::console::object &ob) 
			{ 
				if (args.size() <= 3)
				{
					ob.emit_error(
						FCPPT_TEXT("usage: ")+args[0]+FCPPT_TEXT(" <angle> <latitudes> <longitudes>"));
					return;
				}

				try
				{
					s.regenerate_buffer(
						fcppt::lexical_cast<graphics::scalar>(
							args[1]),
						fcppt::lexical_cast<skydome::size_type>(
							args[2]),
						fcppt::lexical_cast<skydome::size_type>(
							args[3]));
				}
				catch (fcppt::bad_lexical_cast const &)
				{
					ob.emit_error(
						FCPPT_TEXT("Parameter invalid"));
					return;
				}
			},
			FCPPT_TEXT("regenerate skydome")));

	bool running = 
		true;

	fcppt::signal::scoped_connection const cb(
		sys.input_system()->register_callback(
			sge::input::action(
				sge::input::kc::key_escape,
				boost::phoenix::ref(running) = false)));
	
	fcppt::signal::scoped_connection wireframe_conn(
		console.model().insert(
			FCPPT_TEXT("wireframe"),
			[&sys](
				sge::console::arg_list const &args,
				sge::console::object &o)
			{
				if (args.size() == 1)
				{
					o.emit_error(FCPPT_TEXT("usage: ")+args[0]+FCPPT_TEXT(" [0|1]"));
					return;
				}

				if (args[1] == FCPPT_TEXT("1"))
					sys.renderer()->state(
						sge::renderer::state::list
							(sge::renderer::state::draw_mode::line));
				else
					sys.renderer()->state(
						sge::renderer::state::list
							(sge::renderer::state::draw_mode::fill));
			},
			FCPPT_TEXT("Toggle wireframe mode")));

	if (vm.count("wireframe"))
	{
		sys.renderer()->state(
			sge::renderer::state::list
				(sge::renderer::state::draw_mode::line));
	}

	sge::time::timer frame_timer(
		sge::time::second(
			1));
	
	sys.renderer()->state(
		sge::renderer::state::list
		 	(sge::renderer::state::bool_::clear_backbuffer = true)
			(sge::renderer::state::color::clear_color = sge::image::colors::black()));

	while(running)
	{
		sge::mainloop::dispatch();

		cam.update(
			frame_timer.reset());

		sge::renderer::scoped_block const block_(
			sys.renderer());
	
		s.render();
		console.render();
	}
}
catch(sge::exception const &e)
{
	fcppt::io::cerr << e.string() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}
catch (fcppt::exception const &e)
{
	fcppt::io::cerr << e.string() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}
catch(std::exception const &e)
{
	std::cerr << e.what() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}
