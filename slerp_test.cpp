#include "graphics/scalar.hpp"
#include "graphics/vec3.hpp"
#include "graphics/camera/object.hpp"
#include "graphics/camera/cli_options.hpp"
#include "graphics/camera/cli_factory.hpp"
#include "graphics/shader/object.hpp"
#include "graphics/shader/scoped.hpp"
#include "graphics/shader/vf_to_string.hpp"
#include "graphics/cli_options.hpp"
#include "random_engine.hpp"
#include "random_seed.hpp"
#include "console/object.hpp"
#include "get_option.hpp"
#include "media_path.hpp"
#include "input_delegator.hpp"
#include <sge/log/global.hpp>
#include <sge/systems/instance.hpp>
#include <sge/image/create_texture.hpp>
#include <sge/input/key_code.hpp>
#include <sge/input/key_pair.hpp>
#include <sge/systems/list.hpp>
#include <sge/renderer/aspect.hpp>
#include <sge/renderer/vf/format.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/vf/make_unspecified_tag.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/all_extensions.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/renderer/vf/unspecified.hpp>
#include <sge/renderer/vf/vector.hpp>
#include <sge/systems/parameterless.hpp>
#include <sge/systems/image_loader.hpp>
#include <sge/console/arg_list.hpp>
#include <sge/console/object.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
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
#include <sge/input/keyboard/key_event.hpp>
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
#include <fcppt/math/dim/output.hpp>
#include <fcppt/math/dim/input.hpp>
#include <fcppt/math/almost_zero.hpp>
#include <fcppt/math/vector/slerp.hpp>
#include <fcppt/io/cifstream.hpp>
#include <initializer_list>
#include <boost/program_options.hpp>
#include <vector>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <iterator>
#include <ostream>
#include <random>

namespace
{
namespace vf_tags
{
SGE_RENDERER_VF_MAKE_UNSPECIFIED_TAG(position)
SGE_RENDERER_VF_MAKE_UNSPECIFIED_TAG(tip_color)
}

typedef 
sge::renderer::vf::unspecified
<
	sge::renderer::vf::vector
	<
		insula::graphics::scalar,
		3
	>,
	vf_tags::position
> 
vf_position;

typedef 
sge::renderer::vf::unspecified
<
	sge::renderer::vf::vector
	<
		insula::graphics::scalar,
		3
	>,
	vf_tags::tip_color
> 
vf_color;

typedef 
sge::renderer::vf::format
<
	boost::mpl::vector2
	<
		vf_position,
		vf_color
	>
> 
vertex_format;

typedef 
sge::renderer::vf::view
<
	vertex_format
> 
vertex_view;
}

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

	desc.add(
		graphics::cli_options());

	desc.add_options()
		("help","produce help message");
	
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
					get_option<sge::renderer::screen_size>(vm,"graphics-screen-size"),
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
		(sge::systems::parameterless::font)
		(
			sge::systems::image_loader(
				sge::image::capabilities_field::null(),
				sge::all_extensions)));

	console::object console(
		sys.keyboard_collector(),
		sys.renderer(),
		sys.font_system(),
		sys.image_loader(),
		console::redirect_mode::all);

	input_delegator input_delegator_(
		sys,
		console);

	graphics::shader::object shader_(
		sys.renderer(),
		media_path()/FCPPT_TEXT("debug_vertex.glsl"),
		media_path()/FCPPT_TEXT("debug_fragment.glsl"),
		graphics::shader::vf_to_string<vertex_format>(),
		{
		graphics::shader::variable(
			"mvp",
			graphics::shader::variable_type::uniform,
			graphics::mat4())
		},
		graphics::shader::sampler_sequence());
	
	graphics::camera::object cam(
		graphics::camera::cli_factory(
			vm,
			input_delegator_,
			sge::renderer::aspect<graphics::scalar>(
				sys.renderer()->screen_size()),
			graphics::vec3::null()));

	bool running = 
		true;

	fcppt::signal::scoped_connection const cb(
		sys.keyboard_collector()->key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape,
				[&running]() { running = false; })));
	
	sge::time::timer frame_timer(
		sge::time::second(
			1));
	
	sys.renderer()->state(
		sge::renderer::state::list
		 	(sge::renderer::state::bool_::clear_backbuffer = true)
			(sge::renderer::state::color::clear_color = sge::image::colors::black())
			(sge::renderer::state::bool_::clear_zbuffer = true)
		 	(sge::renderer::state::float_::zbuffer_clear_val = 1.f)
			(sge::renderer::state::cull_mode::off)
			(sge::renderer::state::depth_func::less));

	random_engine random(
		random_seed());

	std::uniform_real_distribution<graphics::scalar> rng(
		static_cast<graphics::scalar>(-1000),
		static_cast<graphics::scalar>(1000));

	graphics::vec3 const 
		start = 
			normalize(
				graphics::vec3(
					rng(random),
					rng(random),
					rng(random))),
		end = 
			normalize(
				graphics::vec3(
					rng(random),
					rng(random),
					rng(random)));

	fcppt::io::cout 
		<< "start: " 
		<< start 
		<< ", end: " 
		<< end 
		<< "\n";

	sge::time::timer anim_timer(
		sge::time::second(10),
		sge::time::activation_state::inactive);

	fcppt::signal::scoped_connection start_cb(
		sys.keyboard_collector()->key_callback(
			[&anim_timer](sge::input::keyboard::key_event const &k)
			{
				if (k.key().code() == sge::input::keyboard::key_code::return_ && !anim_timer.active())
				{
					fcppt::io::cout << "Activating animation\n";
					anim_timer.activate();
				}
			}));

	while(running)
	{
		sge::mainloop::dispatch();

		cam.update(
			frame_timer.reset());

		sge::renderer::scoped_block const block_(
			sys.renderer());

		sge::renderer::state::scoped const sstate(
			sys.renderer(),
			sge::renderer::state::list
				(sge::renderer::state::depth_func::off));

		graphics::shader::scoped scoped_shader_(
			shader_);

		shader_.set_uniform(
			FCPPT_TEXT("mvp"),
			cam.mvp());

		sge::renderer::vertex_buffer_ptr vb_ = 
			sys.renderer()->create_vertex_buffer(
				sge::renderer::vf::dynamic::make_format<vertex_format>(),
				static_cast<sge::renderer::size_type>(
					6),
				sge::renderer::resource_flags::none);

		sge::renderer::scoped_vertex_buffer const scoped_vb_(
			sys.renderer(),
			vb_);

		{
			sge::renderer::scoped_vertex_lock const vblock(
				vb_,
				sge::renderer::lock_mode::writeonly);

			vertex_view const vertices(
				vblock.value());

			vertex_view::iterator vb_it(
				vertices.begin());

			graphics::vec3 const current = 
				anim_timer.active() 
				?
					fcppt::math::vector::slerp(
						start,
						end,
						std::min(
							1.0f,
							anim_timer.elapsed_frames()))
				:	
					start;

			// Start arrow
			vb_it->set<vf_position>(
				graphics::vec3::null());
			(vb_it)->set<vf_color>(
				graphics::vec3(1.0f,0.0f,0.0f));
			vb_it++;
			vb_it->set<vf_position>(
				start);
			(vb_it)->set<vf_color>(
				graphics::vec3(1.0f,0.0f,0.0f));
			vb_it++;

			// End arrow
			vb_it->set<vf_position>(
				graphics::vec3::null());
			(vb_it)->set<vf_color>(
				graphics::vec3(0.0f,1.0f,0.0f));
			vb_it++;
			vb_it->set<vf_position>(
				end);
			(vb_it)->set<vf_color>(
				graphics::vec3(0.0f,1.0f,0.0f));
			vb_it++;

			// Current arrow
			vb_it->set<vf_position>(
				graphics::vec3::null());
			(vb_it)->set<vf_color>(
				graphics::vec3(1.0f,1.0f,1.0f));
			vb_it++;
			vb_it->set<vf_position>(
				current);
			(vb_it)->set<vf_color>(
				graphics::vec3(1.0f,1.0f,1.0f));
		}

		sys.renderer()->render(
			sge::renderer::first_vertex(
				0),
			sge::renderer::vertex_count(
				vb_->size()),
			sge::renderer::nonindexed_primitive_type::line);
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
