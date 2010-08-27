#include "graphics/scalar.hpp"
#include "music_controller.hpp"
#include "create_variables_map.hpp"
#include "graphics/vec2.hpp"
#include "graphics/vec3.hpp"
#include "graphics/dim2.hpp"
#include "graphics/camera/object.hpp"
#include "graphics/camera/cli_options.hpp"
#include "graphics/camera/cli_factory.hpp"
#include "graphics/stats.hpp"
#include "graphics/cli_options.hpp"
#include "skydome/object.hpp"
#include "skydome/console_proxy.hpp"
#include "skydome/cli_options.hpp"
#include "skydome/cli_factory.hpp"
#include "height_map/object.hpp"
#include "height_map/console_proxy.hpp"
#include "height_map/cli_factory.hpp"
#include "height_map/cli_options.hpp"
#include "console/object.hpp"
#include "console/scoped_rdbuf.hpp"
#include "water/object.hpp"
#include "water/console_proxy.hpp"
#include "water/cli_options.hpp"
#include "water/cli_factory.hpp"
#include "get_option.hpp"
#include "input_delegator.hpp"
#include <sge/log/global.hpp>
#include <sge/systems/instance.hpp>
#include <sge/systems/audio_loader.hpp>
#include <sge/systems/audio_player_default.hpp>
#include <sge/audio/loader_capabilities_field.hpp>
#include <sge/systems/list.hpp>
#include <sge/systems/parameterless.hpp>
#include <sge/systems/image_loader.hpp>
#include <sge/console/arg_list.hpp>
#include <sge/console/object.hpp>
#include <sge/image/capabilities_field.hpp>
#include <sge/window/parameters.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/viewport.hpp>
#include <sge/renderer/screen_size.hpp>
#include <sge/renderer/display_mode.hpp>
#include <sge/renderer/bit_depth.hpp>
#include <sge/renderer/refresh_rate_dont_care.hpp>
#include <sge/renderer/depth_buffer.hpp>
#include <sge/renderer/stencil_buffer.hpp>
#include <sge/renderer/window_mode.hpp>
#include <sge/renderer/vsync.hpp>
#include <sge/renderer/no_multi_sampling.hpp>
#include <sge/renderer/aspect.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/state/draw_mode.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/color.hpp>
#include <sge/renderer/state/float.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/mainloop/dispatch.hpp>
#include <sge/exception.hpp>
#include <sge/time/timer.hpp>
#include <sge/time/second.hpp>
#include <sge/input/system.hpp>
#include <sge/input/action.hpp>
#include <sge/input/key_code.hpp>
#include <sge/all_extensions.hpp>
// vehicle begin
#include "physics/world.hpp"
#include "physics/cli_options.hpp"
#include "physics/static_model.hpp"
#include "physics/height_map.hpp"
#include "vehicle/object.hpp"
#include "vehicle/cli_options.hpp"
#include "vehicle/cli_factory.hpp"
#include "physics/debug_drawer.hpp"
#include "media_path.hpp"
#include "model/object.hpp"
#include <sge/model/object_ptr.hpp>
#include <sge/audio/loader.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/model/loader_ptr.hpp>
#include <sge/model/loader.hpp>
#include <sge/image/create_texture.hpp>
#include <sge/renderer/texture.hpp>
// vehicle end
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/log/level.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/math/box/center.hpp>
#include <fcppt/math/box/stretch.hpp>
#include <fcppt/math/box/structure_cast.hpp>
#include <fcppt/math/dim/output.hpp>
#include <fcppt/math/dim/input.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/math/vector/input.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/string.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/text.hpp>
#include <fcppt/io/ostream.hpp>
#include <boost/program_options.hpp>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <ostream>

// DEBUG BEGIN
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
// DEBUG END

int main(int argc,char *argv[])
try
{
	using namespace insula;

	fcppt::log::activate_levels(
		sge::log::global(),
		fcppt::log::level::debug);

	/*
	boost::program_options::options_description desc("Allowed options");

	desc.add(
		graphics::camera::cli_options());

	desc.add(
		graphics::cli_options());

	desc.add(
		height_map::cli_options());

	desc.add(
		water::cli_options());

	desc.add(
		skydome::cli_options());

	desc.add(
		vehicle::cli_options());

	desc.add(
		physics::cli_options());
	
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
	*/
	boost::program_options::variables_map const vm = 
		create_variables_map(
			argc,
			argv);

	sge::systems::instance sys(
		sge::systems::list() 
		(
			sge::window::parameters(
				FCPPT_TEXT("render scene")))
		(
			sge::systems::audio_loader(
				sge::audio::loader_capabilities_field::null(),
				fcppt::assign::make_container<sge::extension_set>
					(FCPPT_TEXT("wav"))
					(FCPPT_TEXT("ogg"))))
		(sge::systems::audio_player_default())
		(
			sge::renderer::parameters(
				sge::renderer::display_mode(
					get_option<sge::renderer::screen_size>(vm,"graphics-screen-size"),
					sge::renderer::bit_depth::depth32,
					sge::renderer::refresh_rate_dont_care
				),
				sge::renderer::depth_buffer::d24,
				sge::renderer::stencil_buffer::off,
				sge::renderer::window_mode::windowed,
				sge::renderer::vsync::on,
				sge::renderer::no_multi_sampling))
		(sge::systems::parameterless::input)
		(sge::systems::parameterless::md3_loader)
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

	skydome::object_ptr skydome = 
		skydome::cli_factory(
			vm,
			*cam,
			sys.renderer());

	skydome::console_proxy skydome_console(
		*skydome,
		console.model());

	height_map::object_ptr const terrain = 
		height_map::cli_factory(
			vm,
			*cam,
			sys.renderer(),
			sys.image_loader());

	height_map::console_proxy terrain_console(
		*terrain,
		console.model(),
		sys.image_loader());

	cam->gizmo().position(
		fcppt::math::box::center(
			terrain->extents()) * graphics::vec3(1,1.5,1));

	water::object_ptr water = 
		water::cli_factory(
			vm,
			sys.renderer(),
			*cam,
			fcppt::math::box::structure_cast<graphics::box>(
				terrain->extents()),
			sys.image_loader());

	water::console_proxy water_console(
		*water,
		console.model());

	physics::world physics_world(
		fcppt::math::box::structure_cast<physics::box>(
			terrain->extents()),
		get_option<physics::vec3>(vm,"physics-gravity"));

	physics::height_map physics_height_field(
		physics_world,
		terrain->heights(),
		terrain->cell_size(),
		terrain->height_scaling());

	// vehicle begin
	physics::vec3 physics_vehicle_pos = 
		fcppt::math::vector::structure_cast<physics::vec3>(
			fcppt::math::box::center(
				terrain->extents()) * 
				graphics::vec3(
					static_cast<graphics::scalar>(1.0),
					static_cast<graphics::scalar>(1.1),
					static_cast<graphics::scalar>(1.0)));

	// DEBUG BEGIN
	fcppt::io::cout << "positioning vehicle at " << physics_vehicle_pos << "\n";
	// DEBUG END

	graphics::shader model_shader(
		sys.renderer(),
		media_path()/FCPPT_TEXT("model_vertex.glsl"),
		media_path()/FCPPT_TEXT("model_fragment.glsl"));

	/*
	physics::static_model box_model(
		physics_world,
		physics_vehicle_pos + physics::vec3(0,-5,0),
		model::object_ptr(
			new model::object(
				*cam,
				model_loader->load(
					FCPPT_TEXT("/tmp/cube.md3")),
				sys.renderer(),
				model_shader,
				sge::image::create_texture(
					FCPPT_TEXT("/tmp/stripes.png"),
					sys.renderer(),
					sys.image_loader(),
					sge::renderer::filter::linear,
					sge::renderer::resource_flags::none))),
		physics::model_approximation(
			physics::model_approximation::sphere,
			static_cast<physics::scalar>(1.0)),
		physics::solidity::solid);*/

	vehicle::object_ptr vehicle = 
		vehicle::cli_factory( 
			vm,
			physics_world,
			physics_vehicle_pos,
			sys.renderer(),
			sys.image_loader(),
			sys.md3_loader(),
			model_shader,
			*cam,
			input_delegator_,
			console,
			sys.audio_loader(),
			sys.audio_player());

	// vehicle end

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

	bool physics_debug = false;

	fcppt::signal::scoped_connection physics_debug_conn(
		console.model().insert(
			FCPPT_TEXT("debug_physics"),
			[&sys,&physics_debug](
				sge::console::arg_list const &,
				sge::console::object &)
			{
				physics_debug = !physics_debug;
			},
			FCPPT_TEXT("Toggle the physics debug drawer")));

	sge::time::timer frame_timer(
		sge::time::second(
			1));

	graphics::stats stats(
		sys.renderer(),
		sys.font_system());

	/*
	music_controller music(
		vm,
		sys.audio_loader(),
		sys.audio_player());
*/

	bool show_stats = true;

	console.model().insert(
		FCPPT_TEXT("toggle_stats"),
		[&show_stats](sge::console::arg_list const &,sge::console::object &) 
		{
			show_stats = !show_stats;
		},
		FCPPT_TEXT("Toggle the frame rate display and the vehicle speed"));

	sge::renderer::state::list const global_state = 
		sge::renderer::state::list
			(sge::renderer::state::bool_::clear_zbuffer = true)
			(sge::renderer::state::float_::zbuffer_clear_val = 1.f)
			(sge::renderer::state::bool_::clear_backbuffer = true)
			(sge::renderer::state::color::clear_color = std::get<0>(skydome->gradient()));

	sge::renderer::state::scoped const sstate(
		sys.renderer(),
		global_state);

	// DEBUG BEGIN
	physics::debug_drawer physics_debug_drawer(
		physics_world,
		sys.renderer(),
		*cam);
	// DEBUG END

	while(running)
	{
		sge::mainloop::dispatch();

		sge::time::timer::frames_type const time_delta = 
			frame_timer.reset();

		cam->update(
			time_delta);


		water->update_reflection(
			[&sys,&global_state,&skydome,&terrain,&water]()
			{
				skydome->render();
				terrain->render(
					sge::renderer::state::cull_mode::off,
					water->water_level());
			});

		// FIXME: This is a hack for a bug in renderer
		sys.renderer()->viewport(
			sge::renderer::viewport(
				sge::renderer::pixel_pos::null(),
				sys.renderer()->screen_size()));

		vehicle->update_camera();

		sge::renderer::scoped_block const block_(
			sys.renderer());
	
		skydome->render();
		terrain->render(
			sge::renderer::state::cull_mode::back);
		water->render();

		physics_world.update(
			time_delta);

		// vehicle begin
		vehicle->update();
		vehicle->render();
	//	box_model.render();
		// vehicle end
		
	//	music.update();

		if (physics_debug)
		{
			physics_debug_drawer.setDebugMode(
				btIDebugDraw::DBG_DrawWireframe);
			physics_debug_drawer.render(); 
		}
		
		if (show_stats)
			stats.update_and_render(
				FCPPT_TEXT("Speed: ")+
				fcppt::lexical_cast<fcppt::string>(vehicle->speed_kmh())+
				FCPPT_TEXT("km/h"));
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
