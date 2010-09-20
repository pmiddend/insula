#include "graphics/scalar.hpp"
#include "graphics/vec3.hpp"
#include "graphics/vec2.hpp"
#include "graphics/dim2.hpp"
#include "graphics/camera/object.hpp"
#include "graphics/camera/cli_options.hpp"
#include "graphics/camera/cli_factory.hpp"
#include "graphics/shader/object.hpp"
#include "graphics/shader/vf_to_string.hpp"
#include "graphics/shader/scoped.hpp"
#include "graphics/cli_options.hpp"
#include "graphics/rect.hpp"
#include "console/object.hpp"
#include "gizmo/rotation_to_mat4.hpp"
#include "timed_output.hpp"
#include "model/object.hpp"
#include "model/scoped.hpp"
#include "model/vf/format.hpp"
#include "create_path.hpp"
#include "get_option.hpp"
#include "media_path.hpp"
#include "stdlib/copy.hpp"
#include "input_delegator.hpp"
#include <sge/log/global.hpp>
#include <sge/systems/instance.hpp>
#include <sge/image/create_texture.hpp>
#include <sge/systems/list.hpp>
#include <sge/renderer/aspect.hpp>
#include <sge/renderer/texture_base_ptr.hpp>
#include <sge/systems/parameterless.hpp>
#include <sge/renderer/texture.hpp>
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/depth_stencil_texture.hpp>
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
#include <sge/renderer/vf/format.hpp>
#include <sge/renderer/vf/make_unspecified_tag.hpp>
#include <sge/renderer/vf/unspecified.hpp>
#include <sge/renderer/vf/vector.hpp>
#include <sge/renderer/device.hpp>
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
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/renderer/state/scoped.hpp>
#include <sge/renderer/state/depth_func.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/foreach.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/text.hpp>
#include <fcppt/from_std_string.hpp>
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
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/io/cifstream.hpp>
#include <boost/program_options.hpp>
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
SGE_RENDERER_VF_MAKE_UNSPECIFIED_TAG(mytexcoord)
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
		2
	>,
	vf_tags::mytexcoord
> 
vf_texcoord;

typedef 
sge::renderer::vf::format
<
	boost::mpl::vector2
	<
		vf_position,
		vf_texcoord
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

namespace
{


class quad
{
public:
	explicit 
	quad(
		insula::graphics::rect const &extents,
		sge::renderer::device_ptr _renderer,
		insula::graphics::camera::object &_camera,
		insula::graphics::mat4 const &mvp_sun,
		sge::renderer::texture_ptr test_texture)
	:
		renderer_(
			_renderer),
		camera_(
			_camera),
		shader_(
			renderer_,
			insula::media_path()/FCPPT_TEXT("shadow_vertex.glsl"),
			insula::media_path()/FCPPT_TEXT("shadow_fragment.glsl"),
			insula::graphics::shader::vf_to_string<vertex_format>(),
			{
				insula::graphics::shader::variable(
					"mvp",
					insula::graphics::shader::variable_type::uniform,
					insula::graphics::mat4())
			},
			{}),
		depth_texture_(
			renderer_->create_depth_stencil_texture(
				sge::renderer::dim_type(1024,768),
				sge::renderer::depth_stencil_format::d32)),
		alt_shader_(
			renderer_,
			insula::media_path()/FCPPT_TEXT("shadow_alt_vertex.glsl"),
			insula::media_path()/FCPPT_TEXT("shadow_alt_fragment.glsl"),
			insula::graphics::shader::vf_to_string<vertex_format>(),
			{
				insula::graphics::shader::variable(
					"mvp",
					insula::graphics::shader::variable_type::uniform,
					insula::graphics::mat4()),
				insula::graphics::shader::variable(
					"mvp_sun",
					insula::graphics::shader::variable_type::uniform,
					mvp_sun)
			},
			{
				insula::graphics::shader::sampler(
					"depth_texture",
					depth_texture_)
			}),
		vb_(
			renderer_->create_vertex_buffer(
				sge::renderer::vf::dynamic::make_format<vertex_format>(),
				static_cast<sge::renderer::size_type>(
					6),
				sge::renderer::resource_flags::none))
	{
		insula::graphics::shader::scoped scoped_shader(
			alt_shader_);

		sge::renderer::scoped_vertex_buffer const scoped_vb_(
			renderer_,
			vb_);

		{
			sge::renderer::scoped_vertex_lock const vblock(
				vb_,
				sge::renderer::lock_mode::writeonly);

			vertex_view const vertices(
				vblock.value());

			vertex_view::iterator vb_it(
				vertices.begin());

			insula::graphics::scalar y = -1;

			(vb_it)->set<vf_texcoord>(
				insula::graphics::vec2(
					0,
					0));
			(vb_it++)->set<vf_position>(
				insula::graphics::vec3(
					extents.left(),
					y,
					extents.top()));
			(vb_it)->set<vf_texcoord>(
				insula::graphics::vec2(
					0,
					1));
			(vb_it++)->set<vf_position>(
				insula::graphics::vec3(
					extents.left(),
					y,
					extents.bottom()));
			(vb_it)->set<vf_texcoord>(
				insula::graphics::vec2(
					1,
					1));
			(vb_it++)->set<vf_position>(
				insula::graphics::vec3(
					extents.right(),
					y,
					extents.bottom()));
			(vb_it)->set<vf_texcoord>(
				insula::graphics::vec2(
					1,
					1));
			(vb_it++)->set<vf_position>(
				insula::graphics::vec3(
					extents.right(),
					y,
					extents.bottom()));
			(vb_it)->set<vf_texcoord>(
				insula::graphics::vec2(
					1,
					0));
			(vb_it++)->set<vf_position>(
				insula::graphics::vec3(
					extents.right(),
					y,
					extents.top()));
			(vb_it)->set<vf_texcoord>(
				insula::graphics::vec2(
					0,
					0));
			(vb_it++)->set<vf_position>(
				insula::graphics::vec3(
					extents.left(),
					y,
					extents.top()));
		}
	}

	void
	render()
	{
		insula::graphics::shader::scoped scoped_shader(
			shader_);
		sge::renderer::scoped_vertex_buffer const scoped_vb_(
			renderer_,
			vb_);
		shader_.set_uniform(
			"mvp",
			camera_.mvp());
		renderer_->render(
			sge::renderer::first_vertex(
				0),
			sge::renderer::vertex_count(
				vb_->size()),
			sge::renderer::nonindexed_primitive_type::triangle);
	}

	void
	render_alternative()
	{
		insula::graphics::shader::scoped scoped_shader(
			alt_shader_);
		sge::renderer::scoped_vertex_buffer const scoped_vb_(
			renderer_,
			vb_);
		alt_shader_.set_uniform(
			"mvp",
			camera_.mvp());
		renderer_->render(
			sge::renderer::first_vertex(
				0),
			sge::renderer::vertex_count(
				vb_->size()),
			sge::renderer::nonindexed_primitive_type::triangle);
	}

	sge::renderer::device_ptr renderer_;
	insula::graphics::camera::object &camera_;
	insula::graphics::shader::object shader_;
	sge::renderer::depth_stencil_texture_ptr depth_texture_;
	insula::graphics::shader::object alt_shader_;
	sge::renderer::vertex_buffer_ptr vb_;
};
}

int main(int argc,char *argv[])
try
{
	using namespace insula;

	/*
	fcppt::log::activate_levels(
		sge::log::global(),
		fcppt::log::level::debug
	);
	*/

	boost::program_options::options_description desc("Allowed options");

	desc.add(
		graphics::camera::cli_options());

	desc.add(
		graphics::cli_options());

	desc.add_options()
		("help","produce help message")
		("model",boost::program_options::value<fcppt::string>()->required(),"The model file to load")
		("texture",boost::program_options::value<fcppt::string>(),"The texture")
		("list-parts",boost::program_options::value<bool>()->zero_tokens()->default_value(false),"List all the available parts")
		("part",boost::program_options::value<fcppt::string>(),"Which part of the model we shall load");
	
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
		(sge::systems::parameterless::input)
		(sge::systems::parameterless::md3_loader)
		(sge::systems::parameterless::font)
		(
			sge::systems::image_loader(
				sge::image::capabilities_field::null(),
				fcppt::assign::make_container<sge::extension_set>(FCPPT_TEXT("png")))));

	console::object console(
		sys.input_system(),
		sys.renderer(),
		sys.font_system(),
		sys.image_loader(),
		console::redirect_mode::all);

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

	sge::model::object_ptr const model_object = 
		sys.md3_loader()->load(
			create_path(
				get_option<fcppt::string>(vm,"model"),
				FCPPT_TEXT("models")),
			sge::model::load_flags::switch_yz);

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

	sge::renderer::texture_ptr model_texture = 
		sge::image::create_texture(
				create_path(
					get_option<fcppt::string>(vm,"texture"),
					FCPPT_TEXT("textures")),
				sys.renderer(),
				sys.image_loader(),
				sge::renderer::filter::linear,
				sge::renderer::resource_flags::none);

	graphics::shader::object model_shader(
		sys.renderer(),
		media_path()/FCPPT_TEXT("model_vertex.glsl"),
		media_path()/FCPPT_TEXT("model_fragment.glsl"),
		graphics::shader::vf_to_string<model::vf::format>(),
		fcppt::assign::make_container<graphics::shader::variable_sequence>
		(
			graphics::shader::variable(
				"mvp",
				graphics::shader::variable_type::uniform,
				graphics::mat4())),
		fcppt::assign::make_container<graphics::shader::sampler_sequence>
		(
			graphics::shader::sampler(
				"texture",
				model_texture)));

	model::object model(
		model_object,
		sys.renderer(),
		!vm.count("part")
		?
			fcppt::optional<fcppt::string>()
		:
			fcppt::optional<fcppt::string>(
				get_option<fcppt::string>(vm,"part")));

	bool running = 
		true;

	fcppt::signal::scoped_connection const cb(
		sys.input_system()->register_callback(
			sge::input::action(
				sge::input::kc::key_escape,
				[&running]() { running = false; })));

	// target begin
	insula::graphics::gizmo sun_gizmo(
		insula::graphics::gizmo::init()
			.position(
				insula::graphics::vec3(5.87982,5.8455,4.90575))
			.forward(
				insula::graphics::vec3(0.431345,0.781761,0.450323))
			.right(
				insula::graphics::vec3(0.752727,-0.0366905,-0.657309))
			.up(
				insula::graphics::vec3(-0.497337,0.622497,-0.604279)));

	sge::renderer::texture_ptr const target_texture = 
		sys.renderer()->create_texture(
			sge::renderer::dim_type(1024,1024),
			sge::image::color::format::rgb8,
			sge::renderer::filter::linear,
			sge::renderer::resource_flags::readable);

	quad ground_object(
		insula::graphics::rect(
			insula::graphics::vec2(-5,-5),
			insula::graphics::dim2(10,10)),
		sys.renderer(),
		*cam,
		cam->perspective() * 
		insula::gizmo::rotation_to_mat4(
			sun_gizmo) *
		fcppt::math::matrix::translation(
			sun_gizmo.position() * -1.0f),
		model_texture);

	sge::renderer::target_ptr target = 
		sys.renderer()->create_target(
			/*target_texture*/sge::renderer::texture_ptr(),
			ground_object.depth_texture_);

	// target end
	
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
		 	(sge::renderer::state::float_::zbuffer_clear_val = 1.0f)
		 	(sge::renderer::state::bool_::write_to_zbuffer = true)
			(sge::renderer::state::cull_mode::off)
			(sge::renderer::state::depth_func::less)
			//(sge::renderer::state::depth_func::off)
			);

	{
			insula::graphics::gizmo const old_gizmo = 
				cam->gizmo();

			cam->gizmo() = sun_gizmo;

			sge::renderer::scoped_target const starget(
				sys.renderer(),
				target);

			sge::renderer::scoped_block const block_(
				sys.renderer());

			ground_object.render();


			{
				graphics::shader::scoped scoped_shader(
					model_shader);

				model::scoped scoped_model(
					sys.renderer(),
					model);

				model_shader.set_uniform(
					"mvp",
					cam->perspective() * 
					cam->world());

				model.render();

				model_shader.set_uniform(
					"mvp",
					cam->perspective() * 
					cam->world() * 
					fcppt::math::matrix::translation(
						insula::graphics::vec3(
							0,-4,0)));

				model.render();
			}
			cam->gizmo() = old_gizmo;
		}
	
	//ground_object.depth_texture_->debug();

	while(running)
	{
		sge::mainloop::dispatch();

		cam->update(
			frame_timer.reset());

	//	insula::timed_output() << "cam_pos: " << cam->gizmo().position() << ", forward: " << cam->gizmo().forward() << ", right: " << cam->gizmo().right() << ", up: " << cam->gizmo().up() << "\n";

		

	//	ground_object.depth_texture_->debug();

		{
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
					cam->perspective() * 
					cam->world() *
					fcppt::math::matrix::translation(
						insula::graphics::vec3(
							0,-4,0)));

				model.render();
			}
			ground_object.render_alternative();

			console.render();
		}
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
