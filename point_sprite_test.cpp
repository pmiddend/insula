#include "graphics/scalar.hpp"
#include "random_engine.hpp"
#include "random_seed.hpp"
#include "graphics/vec3.hpp"
#include "graphics/camera/object.hpp"
#include "graphics/camera/parameters.hpp"
#include "graphics/shader/object.hpp"
#include "graphics/shader/scoped.hpp"
#include "graphics/shader/vf_to_string.hpp"
#include "console/object.hpp"
#include "model/object.hpp"
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
#include <sge/renderer/scoped_texture.hpp>
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
#include <sge/parse/json/parse_file_exn.hpp>
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
#include <sge/renderer/vf/format.hpp>
#include <sge/renderer/vertex_buffer.hpp>
#include <sge/renderer/vertex_count.hpp>
#include <sge/renderer/nonindexed_primitive_type.hpp>
#include <sge/renderer/first_vertex.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/glsl/scoped_program.hpp>
#include <sge/renderer/scoped_vertex_buffer.hpp>
#include <sge/renderer/vf/dynamic/make_format.hpp>
#include <sge/renderer/size_type.hpp>
#include <sge/renderer/scoped_vertex_lock.hpp>
#include <sge/renderer/resource_flags_field.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/renderer/lock_mode.hpp>
#include <sge/renderer/dim_type.hpp>
#include <sge/renderer/vf/vertex.hpp>
#include <sge/renderer/vf/iterator.hpp>
#include <sge/renderer/vf/view.hpp>
#include <sge/renderer/vf/make_unspecified_tag.hpp>
#include <sge/renderer/vf/unspecified.hpp>
#include <sge/renderer/vf/vector.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/log/level.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/math/deg_to_rad.hpp>
#include <fcppt/math/twopi.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/string.hpp>
#include <fcppt/char_type.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/io/cifstream.hpp>
#include <boost/program_options.hpp>
#include <initializer_list>
#include <vector>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <iterator>
#include <ostream>

namespace
{
namespace vf_tags
{
SGE_RENDERER_VF_MAKE_UNSPECIFIED_TAG(position)
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
sge::renderer::vf::format
<
	boost::mpl::vector1
	<
		vf_position
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

	sge::parse::json::object const config_file =
		insula::json::config_wrapper(
			{"point_sprite.json"},
			argc,
			argv);

	sge::systems::instance sys(
		sge::systems::list() 
		(
			sge::window::parameters(
				FCPPT_TEXT("test for point sprites")))
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

	graphics::shader::object point_shader(
		sys.renderer(),
		media_path()/FCPPT_TEXT("point_vertex.glsl"),
		media_path()/FCPPT_TEXT("point_fragment.glsl"),
		graphics::shader::vf_to_string<vertex_format>(),
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

	bool running = 
		true;

	sge::renderer::size_type const point_count = 
		json::find_member<sge::renderer::size_type>(
			config_file,
			FCPPT_TEXT("point-count"));

	sge::renderer::vertex_buffer_ptr const vb_ = 
		sys.renderer()->create_vertex_buffer(
			sge::renderer::vf::dynamic::make_format<vertex_format>(),
			point_count,
			sge::renderer::resource_flags::none);

	{
		sge::renderer::glsl::scoped_program scoped_shader_(
			sys.renderer(),
			point_shader.program());

		sge::renderer::scoped_vertex_buffer const scoped_vb_(
			sys.renderer(),
			vb_);

		sge::renderer::scoped_vertex_lock const vblock(
			vb_,
			sge::renderer::lock_mode::writeonly);

		vertex_view const vertices(
			vblock.value());

		vertex_view::iterator vb_it(
			vertices.begin());

		insula::random_engine eng(
			insula::random_seed());

		std::uniform_real_distribution<insula::graphics::scalar> urd(
			-100.0f,
			100.0f);

		for (sge::renderer::size_type i = 0; i < point_count; ++i)
		{
			(vb_it++)->set<vf_position>(
				vf_position::packed_type(
					urd(eng),
					urd(eng),
					urd(eng)));
		}
	}

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
			(sge::renderer::state::color::clear_color = sge::image::colors::black())
	//		(sge::renderer::state::bool_::clear_zbuffer = true)
	//	 	(sge::renderer::state::float_::zbuffer_clear_val = 1.f)
			(sge::renderer::state::cull_mode::off)
	//		(sge::renderer::state::depth_func::less));
			(sge::renderer::state::depth_func::off));

	while(running)
	{
		sge::mainloop::dispatch();

		cam.update(
			frame_timer.reset());

		sge::renderer::scoped_block const block_(
			sys.renderer());

		{
			graphics::shader::scoped scoped_shader_(
				point_shader);

			point_shader.set_uniform(
				FCPPT_TEXT("mvp"),
				cam.mvp());

			sys.renderer()->state(
				sge::renderer::state::list
					(sge::renderer::state::bool_::enable_alpha_blending = true)
					(sge::renderer::state::bool_::enable_point_sprites = true)
					(sge::renderer::state::source_blend_func::src_alpha)
					(sge::renderer::state::dest_blend_func::inv_src_alpha)
);

			sge::renderer::scoped_vertex_buffer const scoped_vb_(
				sys.renderer(),
				vb_);

			sys.renderer()->render(
				sge::renderer::first_vertex(
					0),
				sge::renderer::vertex_count(
					vb_->size()),
				sge::renderer::nonindexed_primitive_type::point);
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
