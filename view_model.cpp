#include "graphics/scalar.hpp"
#include "graphics/vec3.hpp"
#include "graphics/camera/parameters.hpp"
#include "graphics/camera/object.hpp"
#include "graphics/shader/object.hpp"
#include "graphics/shader/vf_to_string.hpp"
#include "graphics/shader/scoped.hpp"
#include "console/object.hpp"
#include "model/object.hpp"
#include "model/scoped.hpp"
#include "model/vf/format.hpp"
#include "create_path.hpp"
#include "json/config_wrapper.hpp"
#include "json/find_member.hpp"
#include "media_path.hpp"
#include "stdlib/copy.hpp"
#include "input_delegator.hpp"
#include <sge/log/global.hpp>
#include <sge/systems/instance.hpp>
#include <sge/image/create_texture.hpp>
#include <sge/systems/list.hpp>
#include <sge/renderer/aspect.hpp>
#include <sge/renderer/texture.hpp>
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
#include <sge/input/keyboard/collector.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/image/multi_loader.hpp>
#include <sge/extension_set.hpp>
#include <sge/model/loader_ptr.hpp>
#include <sge/model/object.hpp>
#include <sge/model/object_ptr.hpp>
#include <sge/model/loader.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/log/level.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/math/twopi.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/string.hpp>
#include <fcppt/char_type.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/text.hpp>
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

	sge::parse::json::object const config_file =
		insula::json::config_wrapper(
			{"view_model.json"},
			argc,
			argv);

	fcppt::log::activate_levels(
		sge::log::global(),
		fcppt::log::level::debug
	);

	sge::systems::instance sys(
		sge::systems::list() 
		(
			sge::window::parameters(
				FCPPT_TEXT("view a model")))
		(
			sge::renderer::parameters(
				sge::renderer::display_mode(
					json::find_member<sge::renderer::screen_size>(
						config_file,
						FCPPT_TEXT("graphics/screen-size")),
					sge::renderer::bit_depth::depth32,
					sge::renderer::refresh_rate_dont_care
				),
				sge::renderer::depth_buffer::d24,
				sge::renderer::stencil_buffer::off,
				sge::renderer::window_mode::windowed,
				sge::renderer::vsync::on,
				sge::renderer::no_multi_sampling))
		(sge::systems::input(
			sge::systems::input_helper_field(
				sge::systems::input_helper::keyboard_collector) 
					| sge::systems::input_helper::mouse_collector))
		(sge::systems::parameterless::md3_loader)
		(sge::systems::parameterless::font)
		(
			sge::systems::image_loader(
				sge::image::capabilities_field::null(),
				{FCPPT_TEXT("png")})));

	console::object console(
		sys.keyboard_collector(),
		sys.renderer(),
		sys.font_system(),
		sys.image_loader(),
		console::redirect_mode::all);

	input_delegator input_delegator_(
		sys,
		console);
	
	graphics::camera::object cam(
		graphics::camera::parameters(
			input_delegator_,
			sge::renderer::aspect<graphics::scalar>(
				sys.renderer()->screen_size()),
			json::find_member<graphics::scalar>(
				config_file,
				FCPPT_TEXT("camera/fov")),
			json::find_member<graphics::scalar>(
				config_file,
				FCPPT_TEXT("camera/near")),
			json::find_member<graphics::scalar>(
				config_file,
				FCPPT_TEXT("camera/far")),
			json::find_member<graphics::scalar>(
				config_file,
				FCPPT_TEXT("camera/movement-speed")),
			json::find_member<graphics::scalar>(
				config_file,
				FCPPT_TEXT("camera/rotation-speed")),
			graphics::vec3::null()));

	sge::model::object_ptr const model_object = 
		sys.md3_loader()->load(
			create_path(
				json::find_member<fcppt::string>(
					config_file,
					FCPPT_TEXT("model")),
				FCPPT_TEXT("models")),
			sge::model::load_flags::switch_yz);

	if (json::find_member<bool>(config_file,FCPPT_TEXT("list-parts")))
	{
		fcppt::io::cout << FCPPT_TEXT("Available model parts are: \n\n");
		stdlib::copy(
			model_object->part_names(),
			std::ostream_iterator<fcppt::string,fcppt::char_type>(
				fcppt::io::cout,
				FCPPT_TEXT("\n")));
		return EXIT_SUCCESS;
	}

	graphics::shader::object model_shader(
		sys.renderer(),
		media_path()/FCPPT_TEXT("model_vertex.glsl"),
		media_path()/FCPPT_TEXT("model_fragment.glsl"),
		graphics::shader::vf_to_string<model::vf::format>(),
		{
			graphics::shader::variable(
				"mvp",
				graphics::shader::variable_type::uniform,
				graphics::mat4())
		},
		{
			graphics::shader::sampler(
				"texture",
				sge::image::create_texture(
				create_path(
					json::find_member<fcppt::string>(
						config_file,
						FCPPT_TEXT("texture")),
					FCPPT_TEXT("textures")),
				sys.renderer(),
				sys.image_loader(),
				sge::renderer::filter::linear,
				sge::renderer::resource_flags::none))
		});

	model::object model(
		model_object,
		sys.renderer(),
		json::find_member<fcppt::string>(
			config_file,
			FCPPT_TEXT("part")).empty()
		?
			fcppt::optional<fcppt::string>()
		:
			fcppt::optional<fcppt::string>(
				json::find_member<fcppt::string>(
					config_file,
					FCPPT_TEXT("part"))));

	bool running = 
		true;
	
	fcppt::signal::scoped_connection const cb(
		sys.keyboard_collector()->key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape,
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

	sge::time::timer frame_timer(
		sge::time::second(
			1));
	
	sys.renderer()->state(
		sge::renderer::state::list
		 	(sge::renderer::state::bool_::clear_backbuffer = true)
			(sge::renderer::state::color::clear_color = sge::image::colors::white())
			(sge::renderer::state::bool_::clear_zbuffer = true)
		 	(sge::renderer::state::float_::zbuffer_clear_val = 1.f)
			//(sge::renderer::state::cull_mode::off)
			//(sge::renderer::state::depth_func::less)
			(sge::renderer::state::depth_func::off)
			);

	while(running)
	{
		sge::mainloop::dispatch();

		cam.update(
			frame_timer.reset());

		sge::renderer::scoped_block const block_(
			sys.renderer());

		{
		graphics::shader::scoped scoped_shader(
			model_shader);

		model::scoped scoped_model(
			sys.renderer(),
			model);

		model_shader.set_uniform(
			"mvp",
			cam.mvp());

		model.render();
		}
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
