#include "graphics/scalar.hpp"
#include "graphics/vec2.hpp"
#include "graphics/vec3.hpp"
#include "graphics/dim2.hpp"
#include "graphics/camera.hpp"
#include "graphics/frame_counter.hpp"
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
// vehicle begin
#include "model/object.hpp"
#include "physics/world.hpp"
#include "physics/sphere.hpp"
#include "physics/height_field.hpp"
#include "physics/scalar.hpp"
#include "physics/vec3.hpp"
#include "physics/vehicle.hpp"
#include "media_path.hpp"
// vehicle end
#include "get_option.hpp"
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
#include <sge/model/loader_ptr.hpp>
#include <sge/model/loader.hpp>
#include <sge/model/object_ptr.hpp>
#include <sge/model/object.hpp>
#include <sge/model/plugin.hpp>
#include <sge/plugin/manager.hpp>
#include <sge/plugin/context.hpp>
#include <sge/plugin/object.hpp>
#include <sge/image/create_texture.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <fcppt/io/cifstream.hpp>
// vehicle end
#include <sge/model/loader_ptr.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/log/level.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/math/box/center.hpp>
#include <fcppt/math/box/stretch.hpp>
#include <fcppt/math/dim/output.hpp>
#include <fcppt/math/dim/input.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/twopi.hpp>
#include <fcppt/string.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/text.hpp>
#include <fcppt/io/ostream.hpp>
#include <boost/program_options.hpp>
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

	desc.add(
		height_map::cli_options());

	desc.add(
		water::cli_options());

	desc.add(
		skydome::cli_options());
	
	desc.add_options()
		("help","produce help message")
		("screen-size",boost::program_options::value<sge::renderer::screen_size>()->default_value(sge::renderer::screen_size(1024,768)),"The size of the screen")
		("fov",boost::program_options::value<graphics::scalar>()->default_value(90),"Field of view (in degrees)")
		("near",boost::program_options::value<graphics::scalar>()->default_value(1.0f),"Distance to the near plane")
		("far",boost::program_options::value<graphics::scalar>()->default_value(10000),"Distance to the far plane")
		("camera-speed",boost::program_options::value<graphics::scalar>()->default_value(500),"Speed of the camera")
		("roll-speed",boost::program_options::value<graphics::scalar>()->default_value(fcppt::math::twopi<graphics::scalar>()/8),"Rolling speed of the camera")
		// vehicle begin
		("chassis-model",boost::program_options::value<fcppt::string>()->required(),"chassis model")
		("chassis-texture",boost::program_options::value<fcppt::string>()->required(),"chassis texture")
		("wheel-model",boost::program_options::value<fcppt::string>()->required(),"Wheel model")
		("wheel-texture",boost::program_options::value<fcppt::string>()->required(),"Wheel texture")
		("vehicle-mass",boost::program_options::value<physics::scalar>()->default_value(static_cast<physics::scalar>(500)),"Mass of the vehicle")
		("chassis-offset",boost::program_options::value<physics::scalar>()->required(),"Vertical offset of the chassis");
		// vehicle end
	
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
				FCPPT_TEXT("render scene")))
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
			get_option<graphics::scalar>(vm,"fov")),
		get_option<graphics::scalar>(vm,"near"),
		get_option<graphics::scalar>(vm,"far"),
		get_option<graphics::scalar>(vm,"camera-speed"),
		graphics::vec3::null());

	skydome::object_ptr skydome = 
		skydome::cli_factory(
			vm,
			cam,
			sys.renderer());

	skydome::console_proxy skydome_console(
		*skydome,
		console.model());

	height_map::object_ptr const terrain = 
		height_map::cli_factory(
			vm,
			cam,
			sys.renderer(),
			sys.image_loader());

	height_map::console_proxy terrain_console(
		*terrain,
		console.model(),
		sys.image_loader());

	// NOTE: The - is needed here, "position" is really not well-defined.
	cam.position(
		-fcppt::math::box::center(
			terrain->extents()));

	water::object_ptr water = 
		water::cli_factory(
			vm,
			sys.renderer(),
			cam,
			fcppt::math::box::stretch(
				graphics::rect(
					graphics::vec2(
						terrain->extents().pos().x(),
						terrain->extents().pos().z()),
					graphics::dim2(
						terrain->extents().dimension().w(),
						terrain->extents().dimension().d())),
					static_cast<graphics::scalar>(
						2)),
			sys.image_loader());

	water::console_proxy water_console(
		*water,
		console.model());

	physics::world physics_world(
		// FIXME: structure_cast!
		terrain->extents());

	physics::height_field physics_height_field(
		physics_world,
		terrain->heights(),
		terrain->cell_size(),
		terrain->height_scaling());

	// vehicle begin

	graphics::shader model_shader(
		sys.renderer(),
		media_path()/FCPPT_TEXT("model_vertex.glsl"),
		media_path()/FCPPT_TEXT("model_fragment.glsl"));

	sge::plugin::object<sge::model::loader>::ptr_type const model_plugin(
		sys.plugin_manager().plugin<sge::model::loader>().load()); 

	sge::model::loader_ptr const loader(
		model_plugin->get()()); 

	fcppt::io::cifstream ifs_chassis_model(
		get_option<fcppt::string>(vm,"chassis-model"),
		std::ios_base::binary);

	fcppt::io::cifstream ifs_wheel_model(
		get_option<fcppt::string>(vm,"wheel-model"),
		std::ios_base::binary);

	sge::model::object_ptr const chassis_model_object = 
		loader->load(
			ifs_chassis_model);

	sge::model::object_ptr const wheel_model_object = 
		loader->load(
			ifs_wheel_model);

	FCPPT_ASSERT(
		chassis_model_object->part_names().size());

	sge::model::scalar const vehicle_scaling = 
		static_cast<sge::model::scalar>(
			20);

	model::object chassis_model(
		chassis_model_object->part_names()[0],
		cam,
		chassis_model_object,
		sys.renderer(),
		model_shader,
		sge::image::create_texture(
			get_option<fcppt::string>(vm,"chassis-texture"),
			sys.renderer(),
			sys.image_loader(),
			sge::renderer::filter::linear,
			sge::renderer::resource_flags::none),
		vehicle_scaling);

	FCPPT_ASSERT(
		wheel_model_object->part_names().size());

	model::object wheel_model(
		wheel_model_object->part_names()[0],
		cam,
		wheel_model_object,
		sys.renderer(),
		model_shader,
		sge::image::create_texture(
			get_option<fcppt::string>(vm,"wheel-texture"),
			sys.renderer(),
			sys.image_loader(),
			sge::renderer::filter::linear,
			sge::renderer::resource_flags::none),
		vehicle_scaling);

	physics::vec3 physics_vehicle_pos = 
		fcppt::math::vector::structure_cast<physics::vec3>(
			fcppt::math::box::center(
				terrain->extents()));

	physics_vehicle_pos[1] = terrain->extents().dimension()[1];

	physics::scalar const wheel_offset = static_cast<physics::scalar>(-0.8);

	physics::wheel_info_sequence wheels;
	wheels.push_back(
		physics::wheel_info()
			.position(
				vehicle_scaling * physics::vec3(
					0.9f,
					wheel_offset,
					1.2f))
			.is_front_wheel()
			.gets_steering()
			);
	wheels.push_back(
		physics::wheel_info()
			.position(
				vehicle_scaling * physics::vec3(
					-0.9f,
					wheel_offset,
					1.2f))
			.is_front_wheel()
			.gets_steering()
			);
	wheels.push_back(
		physics::wheel_info()
			.position(
				vehicle_scaling * physics::vec3(
					0.9f,
					wheel_offset,
					-0.85f))
			.gets_engine_force()
			.gets_breaking_force()
			);
	wheels.push_back(
		physics::wheel_info()
			.position(
				vehicle_scaling * physics::vec3(
					-0.9f,
					wheel_offset,
					-0.85f))
			.gets_engine_force()
			.gets_breaking_force()
			);

	physics::vehicle vehicle(
		physics_world,
		sys.renderer(),
		chassis_model,
		get_option<physics::scalar>(vm,"vehicle-mass"),
		vehicle_scaling * get_option<physics::scalar>(vm,"chassis-offset"),
		physics_vehicle_pos, 
		// max engine force
		100,
		// max breaking force
		100,
		wheel_model,
		wheels);
	// vehicle end

	/*
	physics::vec3 physics_sphere_pos = 
		fcppt::math::vector::structure_cast<physics::vec3>(
			fcppt::math::box::center(
				terrain->extents()));

	physics_sphere_pos[1] = terrain->extents().dimension()[1];

	physics::sphere physics_sphere(
		sys.renderer(),
		cam,
		physics_world,
		static_cast<physics::scalar>(
			50),
		static_cast<physics::scalar>(
			5000),
		physics_sphere_pos);
	*/

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

	graphics::frame_counter frame_counter(
		sys.renderer(),
		sys.font_system());

	bool show_fps = true;

	console.model().insert(
		FCPPT_TEXT("toggle_fps"),
		[&show_fps](sge::console::arg_list const &,sge::console::object &) 
		{
			show_fps = !show_fps;
		},
		FCPPT_TEXT("Toggle the frame rate display"));

	sge::renderer::state::list const global_state = 
		sge::renderer::state::list
			(sge::renderer::state::bool_::clear_zbuffer = true)
			(sge::renderer::state::float_::zbuffer_clear_val = 1.f)
			(sge::renderer::state::bool_::clear_backbuffer = true)
			(sge::renderer::state::color::clear_color = std::get<0>(skydome->gradient()));

	sge::renderer::state::scoped const sstate(
		sys.renderer(),
		global_state);

	while(running)
	{
		sge::mainloop::dispatch();

		sge::time::timer::frames_type const time_delta = 
			frame_timer.reset();

		cam.update(
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

		sge::renderer::scoped_block const block_(
			sys.renderer());
	
		skydome->render();
		terrain->render(
			sge::renderer::state::cull_mode::back);
		water->render();

		physics_world.update(
			time_delta);

		// vehicle begin
		vehicle.update();
		vehicle.render();
		// vehicle end
		
		if (show_fps)
			frame_counter.update_and_render();
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
