#include "height_map/array.hpp"
#include "height_map/scalar.hpp"
#include "height_map/vector2.hpp"
#include "height_map/object.hpp"
#include "height_map/image_to_array.hpp"
#include "height_map/generate_gradient.hpp"
#include "height_map/normalize_and_stretch.hpp"
#include "graphics/shader.hpp"
#include "graphics/camera.hpp"
#include "graphics/vec2.hpp"
#include "graphics/scalar.hpp"
#include "textures/image_sequence.hpp"
#include "console/object.hpp"
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/config/media_path.hpp>
#include <sge/image/file.hpp>
#include <sge/renderer/refresh_rate_dont_care.hpp>
#include <sge/renderer/no_multi_sampling.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/system.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/filter/trilinear.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/glsl/uniform/variable_ptr.hpp>
#include <sge/renderer/glsl/uniform/single_value.hpp>
#include <sge/renderer/glsl/program.hpp>
#include <sge/input/system.hpp>
#include <sge/input/action.hpp>
#include <sge/time/timer.hpp>
#include <sge/image/multi_loader.hpp>
#include <sge/image/capabilities.hpp>
#include <sge/image/colors.hpp>

#include <sge/time/millisecond.hpp>
#include <sge/time/second.hpp>
#include <sge/time/default_callback.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/draw_mode.hpp>
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/color.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/image/colors.hpp>
#include <sge/mainloop/dispatch.hpp>
#include <sge/console/object.hpp>
#include <sge/log/global.hpp>
#include <sge/all_extensions.hpp>
#include <sge/exception.hpp>
#include <mizuiro/image/make_const_view.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/log/level.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/output.hpp>
#include <fcppt/math/matrix/transpose.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/spirit/home/phoenix/core/reference.hpp>
#include <boost/spirit/home/phoenix/operator/self.hpp>
#include <boost/program_options.hpp>
#include <cstdlib>
#include <exception>
#include <algorithm>
#include <iterator>
#include <ostream>
#include <vector>

int main(int argc,char *argv[])
try
{
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
		("fov",boost::program_options::value<insula::graphics::scalar>()->default_value(90),"Field of view (in degrees)")
		("near",boost::program_options::value<insula::graphics::scalar>()->default_value(1),"Distance to the near plane")
		("far",boost::program_options::value<insula::graphics::scalar>()->default_value(10000),"Distance to the far plane")
		("grid-x",boost::program_options::value<insula::height_map::scalar>()->default_value(20),"Size of a grid cell in x dimension")
		("grid-y",boost::program_options::value<insula::height_map::scalar>()->default_value(20),"Size of a grid cell in y dimension")
		("height-scale",boost::program_options::value<insula::height_map::scalar>()->default_value(1000),"Height scaling")
		("camera-speed",boost::program_options::value<insula::graphics::scalar>()->default_value(500),"Speed of the camera")
		("height-map",boost::program_options::value<fcppt::string>()->required(),"Height map (has to be greyscale)")
		("gradient-texture",boost::program_options::value<fcppt::string>()->required(),"Texture for the gradient")
		("height-texture",boost::program_options::value<string_vector>(&height_textures)->multitoken(),"Height texture")
		("ambient-light",boost::program_options::value<insula::graphics::scalar>()->default_value(static_cast<insula::graphics::scalar>(0.4)),"Ambient lighting (in [0,1])")
		("sun-x",boost::program_options::value<insula::graphics::scalar>()->default_value(static_cast<insula::graphics::scalar>(100)),"Sun x position")
		("sun-y",boost::program_options::value<insula::graphics::scalar>()->default_value(static_cast<insula::graphics::scalar>(1000)),"Sun y position")
		("sun-z",boost::program_options::value<insula::graphics::scalar>()->default_value(static_cast<insula::graphics::scalar>(100)),"Sun z position")
		("texture-scaling",boost::program_options::value<insula::graphics::scalar>()->default_value(static_cast<insula::graphics::scalar>(20)),"Texture scaling (the higher the value, the more often the texture is repeating)")
		("wireframe",boost::program_options::value<bool>()->zero_tokens(),"Enable wireframe mode");
	
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

	insula::console::object console(
		sys.input_system(),
		sys.renderer(),
		sys.font_system(),
		sys.image_loader());
	
	insula::height_map::array height_map_array_raw = 
		insula::height_map::image_to_array(
			sys.image_loader().load(
				filename));
	
	insula::height_map::array height_map_array = 
		height_map_array_raw;

	insula::height_map::normalize_and_stretch(
		height_map_array);

	insula::height_map::array grad = 
		insula::height_map::generate_gradient(
			height_map_array);

	insula::height_map::normalize_and_stretch(
		grad);
	
	std::transform(
		grad.data(),
		grad.data() + height_map_array.num_elements(),
		grad.data(),
		[](insula::height_map::array::element const s) { return std::sin(s); });

	insula::height_map::object h(
		sys.renderer(),
		height_map_array_raw,
		height_map_array,
		grad,
		vm["height-scale"].as<insula::height_map::scalar>(),
		insula::height_map::vector2(
			vm["grid-x"].as<insula::height_map::scalar>(),
			vm["grid-y"].as<insula::height_map::scalar>()));

	sge::image::file_ptr const gradient_image = 
		sys.image_loader().load(
			vm["gradient-texture"].as<fcppt::string>());

	insula::textures::image_sequence images;
	
	std::transform(
		height_textures.begin(),
		height_textures.end(),
		std::back_inserter<insula::textures::image_sequence>(
			images),
		[&sys](fcppt::filesystem::path const &p) { return sys.image_loader().load(p); });

	sge::renderer::texture_ptr const 
		sand_texture = 
			sys.renderer()->create_texture(
				images[0]->view(),
				sge::renderer::filter::trilinear,
				sge::renderer::resource_flags::none),
		rock_texture = 
			sys.renderer()->create_texture(
				gradient_image->view(),
				sge::renderer::filter::trilinear,
				sge::renderer::resource_flags::none),
		grass_texture = 
			sys.renderer()->create_texture(
				images[1]->view(),
				sge::renderer::filter::trilinear,
				sge::renderer::resource_flags::none);

	bool running = 
		true;

	fcppt::signal::scoped_connection const cb(
		sys.input_system()->register_callback(
			sge::input::action(
				sge::input::kc::key_escape,
				boost::phoenix::ref(running) = false)));
	
	if (vm.count("wireframe"))
	{
		sys.renderer()->state(
			sge::renderer::state::list
				(sge::renderer::state::draw_mode::line));
	}
	
	insula::graphics::shader terrain_shader(
		sys.renderer(),
		FCPPT_TEXT("media/vertex.glsl"),
		FCPPT_TEXT("media/fragment.glsl"));
	
	terrain_shader.activate();
	
	terrain_shader.set_uniform(
		FCPPT_TEXT("sand"),
		0);

	terrain_shader.set_uniform(
		FCPPT_TEXT("rock"),
		1);

	terrain_shader.set_uniform(
		FCPPT_TEXT("grass"),
		2);
	
	terrain_shader.set_uniform(
		FCPPT_TEXT("sun_direction"),
		insula::graphics::vec3(
			vm["sun-x"].as<insula::graphics::scalar>(),
			vm["sun-y"].as<insula::graphics::scalar>(),
			vm["sun-z"].as<insula::graphics::scalar>()));

	terrain_shader.set_uniform(
		FCPPT_TEXT("ambient_light"),
		vm["ambient-light"].as<insula::graphics::scalar>());

	terrain_shader.set_uniform(
		FCPPT_TEXT("multiplicator"),
		vm["texture-scaling"].as<insula::graphics::scalar>());
	
	terrain_shader.set_uniform(
		FCPPT_TEXT("grid_size"),
		insula::graphics::vec2(
			static_cast<insula::graphics::scalar>(
				static_cast<insula::graphics::scalar>(images[0]->dim()[0]) * 
				vm["grid-x"].as<insula::height_map::scalar>()),
			static_cast<insula::graphics::scalar>(
				static_cast<insula::graphics::scalar>(images[0]->dim()[1]) * 
				vm["grid-y"].as<insula::height_map::scalar>())));

	insula::graphics::camera cam(
		console,
		static_cast<insula::graphics::scalar>(
			1024.0/768.0),
		static_cast<insula::graphics::scalar>(
			fcppt::math::deg_to_rad(
				static_cast<insula::graphics::scalar>(
					vm["fov"].as<insula::graphics::scalar>()))),
		vm["near"].as<insula::graphics::scalar>(),
		vm["far"].as<insula::graphics::scalar>(),
		vm["camera-speed"].as<insula::graphics::scalar>(),
		insula::graphics::vec3::null());
	
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

		terrain_shader.activate();
	
		terrain_shader.set_uniform(
			FCPPT_TEXT("world"),
			fcppt::math::matrix::transpose(
				cam.world()));

		terrain_shader.set_uniform(
			FCPPT_TEXT("perspective"),
			fcppt::math::matrix::transpose(
				cam.perspective()));

		sge::renderer::scoped_block const block_(
			sys.renderer());
	
		sys.renderer()->texture(
			sand_texture,
			0);

		sys.renderer()->texture(
			rock_texture,
			1);

		sys.renderer()->texture(
			grass_texture,
			2);

		h.render();

		sys.renderer()->glsl_program(
			sge::renderer::glsl::program_ptr());

		sys.renderer()->texture(
			sge::renderer::device::no_texture,
			0);

		sys.renderer()->texture(
			sge::renderer::device::no_texture,
			1);

		sys.renderer()->texture(
			sge::renderer::device::no_texture,
			2);
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
	fcppt::io::cerr << e.what() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}
