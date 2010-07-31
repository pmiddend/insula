#include "graphics/scalar.hpp"
#include "graphics/vec2.hpp"
#include "graphics/vec3.hpp"
#include "graphics/camera.hpp"
#include "height_map/image_to_array.hpp"
#include "height_map/object.hpp"
#include "console/object.hpp"
#include <sge/log/global.hpp>
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/renderer/aspect.hpp>
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
#include <fcppt/math/twopi.hpp>
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
		("grid-sizes",boost::program_options::value<graphics::vec2>()->default_value(graphics::vec2(20,20)),"Size of a grid cell")
		("height-scale",boost::program_options::value<graphics::scalar>()->default_value(1000),"Height scaling")
		("camera-speed",boost::program_options::value<graphics::scalar>()->default_value(500),"Speed of the camera")
		("roll-speed",boost::program_options::value<graphics::scalar>()->default_value(fcppt::math::twopi<graphics::scalar>()/8),"Rolling speed of the camera")
		("height-map",boost::program_options::value<fcppt::string>()->required(),"Height map (has to be greyscale)")
		("gradient-texture",boost::program_options::value<fcppt::string>()->required(),"Texture for the gradient")
		("height-texture",boost::program_options::value<string_vector>(&height_textures)->multitoken(),"Height texture")
		("ambient-light",boost::program_options::value<graphics::scalar>()->default_value(static_cast<graphics::scalar>(0.4)),"Ambient lighting (in [0,1])")
		("sun-direction",boost::program_options::value<graphics::vec3>()->default_value(graphics::vec3(100,100,100)),"Sun direction")
		("texture-scaling",boost::program_options::value<graphics::scalar>()->default_value(static_cast<graphics::scalar>(20)),"Texture scaling (the higher the value, the more often the texture is repeating)")
		("clip",boost::program_options::value<bool>()->default_value(false),"Clip at the water level")
		("water-level",boost::program_options::value<graphics::scalar>()->default_value(static_cast<graphics::scalar>(5)),"Water level to clip at (has no effect if clipping is disabled)");
	
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

	fcppt::filesystem::path const filename(
		vm["height-map"].as<fcppt::string>());

	sge::systems::instance sys(
		sge::systems::list() 
		(
			sge::window::parameters(
				FCPPT_TEXT("render height map")))
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
		sge::renderer::aspect<graphics::scalar>(
			sys.renderer()->screen_size()),
		fcppt::math::deg_to_rad(
				vm["fov"].as<graphics::scalar>()),
		vm["near"].as<graphics::scalar>(),
		vm["far"].as<graphics::scalar>(),
		vm["camera-speed"].as<graphics::scalar>(),
		graphics::vec3::null());

	height_map::object h(
		cam,
		sys.renderer(),
		console.model(),
		height_map::image_to_array(
			sys.image_loader().load(
				filename)),
		vm["grid-sizes"].as<graphics::vec2>(),
		vm["height-scale"].as<graphics::scalar>(),
		vm["sun-direction"].as<graphics::vec3>(),
		vm["ambient-light"].as<graphics::scalar>(),
		vm["texture-scaling"].as<graphics::scalar>(),
		vm["water-level"].as<graphics::scalar>(),
		sys.image_loader().load(
			vm["gradient-texture"].as<fcppt::string>()),
		sys.image_loader().load(
			height_textures[0]),
		sys.image_loader().load(
			height_textures[1]));
	
	fcppt::signal::scoped_connection regenerate_conn(
		console.model().insert(
			FCPPT_TEXT("regenerate"),
			[&h,&sys](sge::console::arg_list const &args,sge::console::object &ob) 
			{ 
				if (args.size() <= 4)
				{
					ob.emit_error(
						FCPPT_TEXT("usage: ")+args[0]+FCPPT_TEXT(" <filename> <cell-size-x> <cell-size-y> <height-scaling>"));
					return;
				}

				if (!fcppt::filesystem::is_regular(args[1]))
				{
					ob.emit_error(
						FCPPT_TEXT("File \"")+args[1]+FCPPT_TEXT("\" isn't regular"));
					return;
				}

				try
				{
					h.regenerate(
						graphics::vec2(
							fcppt::lexical_cast<graphics::scalar>(
							args[2]),
							fcppt::lexical_cast<graphics::scalar>(
								args[3])),
						fcppt::lexical_cast<graphics::scalar>(
							args[4]),
						height_map::image_to_array(
							sys.image_loader().load(
								args[1])));
				}
				catch (fcppt::bad_lexical_cast const &)
				{
					ob.emit_error(
						FCPPT_TEXT("Cell size/height scaling invalid"));
					return;
				}
			},
			FCPPT_TEXT("Regenerate terrain from a file"),
			FCPPT_TEXT("Usage: /regenerate <filename> <cell-size-x> <cell-size-y> <height-scaling>\nThe filename should exist and denote a grey-scale image.")));

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
			(sge::renderer::state::color::clear_color = sge::image::colors::black())
			(sge::renderer::state::bool_::clear_zbuffer = true)
		 	(sge::renderer::state::float_::zbuffer_clear_val = 1.f));

	while(running)
	{
		sge::mainloop::dispatch();

		cam.update(
			frame_timer.reset());

		sge::renderer::scoped_block const block_(
			sys.renderer());
	
		h.render(
			vm["clip"].as<bool>() ? height_map::render_mode::clip : height_map::render_mode::none);
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
