#include "graphics/scalar.hpp"
#include "graphics/vec3.hpp"
#include "graphics/camera/object.hpp"
#include "graphics/camera/cli_options.hpp"
#include "graphics/camera/cli_factory.hpp"
#include "graphics/shader.hpp"
#include "console/object.hpp"
#include "model/object.hpp"
#include "get_option.hpp"
#include "media_path.hpp"
#include "stdlib/copy.hpp"
#include "input_delegator.hpp"
#include <sge/log/global.hpp>
#include <sge/systems/instance.hpp>
#include <sge/image/create_texture.hpp>
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
#include <sge/extension_set.hpp>
#include <sge/plugin/manager.hpp>
#include <sge/plugin/object.hpp>
#include <sge/model/loader_ptr.hpp>
#include <sge/model/object.hpp>
#include <sge/model/object_ptr.hpp>
#include <sge/model/loader.hpp>
#include <sge/model/plugin.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/filter/linear.hpp>
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
#include <fcppt/char_type.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/dim/output.hpp>
#include <fcppt/math/dim/input.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/io/cifstream.hpp>
#include <boost/program_options.hpp>
#include <vector>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <iterator>
#include <ostream>

int main(int argc,char *argv[])
try
{
	using namespace insula;

	fcppt::log::activate_levels(
		sge::log::global(),
		fcppt::log::level::debug
	);

	boost::program_options::options_description desc("Allowed options");

	desc.add(
		graphics::camera::cli_options());

	desc.add_options()
		("help","produce help message")
		("model",boost::program_options::value<fcppt::string>()->required(),"The model file to load")
		("texture",boost::program_options::value<fcppt::string>(),"The texture")
		("list-parts",boost::program_options::value<bool>()->zero_tokens()->default_value(false),"List all the available parts")
		("part",boost::program_options::value<fcppt::string>(),"Which part of the model we shall load")
		("screen-size",boost::program_options::value<sge::renderer::screen_size>()->default_value(sge::renderer::screen_size(1024,768)),"The size of the screen");
	
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
				FCPPT_TEXT("view a model")))
		(
			sge::renderer::parameters(
				sge::renderer::display_mode(
					get_option<sge::renderer::screen_size>(vm,"screen-size"),
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
				fcppt::assign::make_container<sge::extension_set>(FCPPT_TEXT("png")))));

	console::object console(
		sys.input_system(),
		sys.renderer(),
		sys.font_system(),
		sys.image_loader());

	input_delegator input_delegator_(
		sys.input_system(),
		console);
	
	graphics::camera::object_ptr cam = 
		graphics::camera::cli_factory(
			vm,
			input_delegator_,
			sge::renderer::aspect<graphics::scalar>(
				sys.renderer()->screen_size()),
			graphics::vec3::null());

	graphics::shader model_shader(
		sys.renderer(),
		media_path()/FCPPT_TEXT("model_vertex.glsl"),
		media_path()/FCPPT_TEXT("model_fragment.glsl"));

	
	sge::plugin::object<sge::model::loader>::ptr_type const model_plugin(
		sys.plugin_manager().plugin<sge::model::loader>().load()); 

	sge::model::loader_ptr const loader(
		model_plugin->get()()); 

	fcppt::io::cifstream ifs(
		get_option<fcppt::string>(vm,"model"),
		std::ios_base::binary);
	
	sge::model::object_ptr const model_object = 
		loader->load(
			ifs);

	if (get_option<bool>(vm,"list-parts"))
	{
		fcppt::io::cout << FCPPT_TEXT("Available model parts are: \n\n");
		stdlib::copy(
			model_object->part_names(),
			std::ostream_iterator<fcppt::string,fcppt::char_type>(
				fcppt::io::cout,
				FCPPT_TEXT("\n")));
		return EXIT_SUCCESS;
	}

	if (!vm.count("texture"))
	{
		fcppt::io::cerr << FCPPT_TEXT("You have to specify a texture!\n");
		return EXIT_FAILURE;
	}

	model::object model(
		*cam,
		model_object,
		sys.renderer(),
		model_shader,
		sge::image::create_texture(
			get_option<fcppt::string>(vm,"texture"),
			sys.renderer(),
			sys.image_loader(),
			sge::renderer::filter::linear,
			sge::renderer::resource_flags::none),
		get_option<fcppt::string>(vm,"part"));

	bool running = 
		true;

	fcppt::signal::scoped_connection const cb(
		sys.input_system()->register_callback(
			sge::input::action(
				sge::input::kc::key_escape,
				[&running]() { running = false; })));
	
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
			(sge::renderer::state::color::clear_color = sge::image::colors::white())
			(sge::renderer::state::bool_::clear_zbuffer = true)
		 	(sge::renderer::state::float_::zbuffer_clear_val = 1.f)
			(sge::renderer::state::cull_mode::off)
			(sge::renderer::state::depth_func::less));

	while(running)
	{
		sge::mainloop::dispatch();

		cam->update(
			frame_timer.reset());

		sge::renderer::scoped_block const block_(
			sys.renderer());

		model.render(
			graphics::mat4::identity());
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
	std::cerr << e.what() << '\n';
	return EXIT_FAILURE;
}
