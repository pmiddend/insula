#include "machine.hpp"
#include "application_title.hpp"
#include "get_option.hpp"
#include "graphics/camera/cli_factory.hpp"
// Camera has to be updated in tick
#include "graphics/camera/object.hpp"
#include "graphics/scalar.hpp"
#include "graphics/vec3.hpp"
#include "events/tick.hpp"
#include "events/render.hpp"
#include "states/game_outer.hpp"
#include <sge/window/parameters.hpp>
#include <sge/systems/audio_player_default.hpp>
#include <sge/renderer/parameters.hpp>
#include <sge/renderer/display_mode.hpp>
#include <sge/renderer/screen_size.hpp>
#include <sge/renderer/aspect.hpp>
#include <sge/renderer/bit_depth.hpp>
#include <sge/renderer/refresh_rate_dont_care.hpp>
#include <sge/renderer/device.hpp>
#include <sge/renderer/depth_buffer.hpp>
#include <sge/renderer/stencil_buffer.hpp>
#include <sge/renderer/window_mode.hpp>
#include <sge/renderer/vsync.hpp>
#include <sge/renderer/scoped_block.hpp>
#include <sge/renderer/no_multi_sampling.hpp>
// For the wireframe callback begin
#include <sge/console/object.hpp>
#include <sge/console/arg_list.hpp>
#include <sge/renderer/state/list.hpp>
#include <sge/renderer/state/draw_mode.hpp>
// For the exit callback begin
#include <sge/input/system.hpp>
#include <sge/input/action.hpp>
#include <sge/input/key_code.hpp>
// For the exit callback end
#include <sge/systems/parameterless.hpp>
#include <sge/systems/image_loader.hpp>
#include <sge/systems/list.hpp>
// Default state begin
#include <sge/renderer/state/bool.hpp>
#include <sge/renderer/state/float.hpp>
#include <sge/renderer/state/color.hpp>
#include <sge/renderer/state/trampoline.hpp>
#include <sge/image/colors.hpp>
// Default state end
#include <sge/extension_set.hpp>
#include <sge/all_extensions.hpp>
#include <sge/image/capabilities_field.hpp>
// Config file begin
#include <sge/parse/json/parse_file_exn.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include "media_path.hpp"
// Config file end
#include <fcppt/text.hpp>
// Viewport hack begin
#include <sge/renderer/viewport.hpp>
#include <sge/renderer/pixel_pos.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
// Viewport hack end
#include <sge/mainloop/dispatch.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/math/vector/basic_impl.hpp>

insula::machine::machine(
	boost::program_options::variables_map const &_vm)
:
	// Has to be initialized early because the exit callback needs it
	running_(true),
	config_file_(
		sge::parse::json::parse_file_exn(
			media_path()/FCPPT_TEXT("config.json"))),
	vm_(
		_vm),
	systems_(
		sge::systems::list
		(
			sge::window::parameters(
				application_title()))
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
					get_option<sge::renderer::screen_size>(
						vm_,
						"graphics-screen-size"),
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
				sge::all_extensions))),
	console_(
		systems_.input_system(),
		systems_.renderer(),
		systems_.font_system(),
		systems_.image_loader()),
	input_delegator_(
		systems_.input_system(),
		console_),
	camera_(
		graphics::camera::cli_factory(
			vm_,
			input_delegator_,
			sge::renderer::aspect<graphics::scalar>(
				systems_.renderer()->screen_size()),
			graphics::vec3::null())),
	exit_callback_(
		systems_.input_system()->register_callback(
			sge::input::action(
				sge::input::kc::key_escape,
				[&running_]() { running_ = false; }))),
	wireframe_callback_(
		console_.model().insert(
			FCPPT_TEXT("wireframe"),
			[&systems_](
				sge::console::arg_list const &args,
				sge::console::object &o)
			{
				if (args.size() == 1)
				{
					o.emit_error(
						FCPPT_TEXT("usage: ")+
						args[0]+
						FCPPT_TEXT(" [0|1]"));
					return;
				}

				if (args[1] == FCPPT_TEXT("1"))
					systems_.renderer()->state(
						sge::renderer::state::list
							(sge::renderer::state::draw_mode::line));
				else
					systems_.renderer()->state(
						sge::renderer::state::list
							(sge::renderer::state::draw_mode::fill));
			},
			FCPPT_TEXT("Toggle wireframe mode"))),
	stats_(
		systems_.renderer(),
		systems_.font_system()),
	show_stats_(
		true),
	stats_callback_(
		console_.model().insert(
			FCPPT_TEXT("toggle_stats"),
			[&show_stats_](
				sge::console::arg_list const &,
				sge::console::object &)
			{
				show_stats_ = !show_stats_;
			},
			FCPPT_TEXT("Toggle display of statistics"))),
	music_(
		sge::parse::json::find_member_exn<sge::parse::json::object>(
			config_file_.members,
			FCPPT_TEXT("music")),
		systems_.audio_loader(),
		systems_.audio_player()),
	sounds_(
		sge::parse::json::find_member_exn<sge::parse::json::object>(
			config_file_.members,
			FCPPT_TEXT("sounds")),
		systems_.audio_loader(),
		systems_.audio_player())
{
	// Set a sensible default renderer state (as a basis, so to speak)
	systems_.renderer()->state(
		sge::renderer::state::list
			(sge::renderer::state::bool_::clear_zbuffer = true)
			(sge::renderer::state::float_::zbuffer_clear_val = 1.f)
			(sge::renderer::state::bool_::clear_backbuffer = true)
			(sge::renderer::state::color::clear_color = sge::image::colors::black()));
}

void
insula::machine::tick(
	sge::time::timer::frames_type const time_delta)
{
	sge::mainloop::dispatch();

	camera_->update(
		time_delta);

	process_event(
		events::tick(
			time_delta));

	// FIXME: This is a hack for a bug in renderer
	systems_.renderer()->viewport(
		sge::renderer::viewport(
			sge::renderer::pixel_pos::null(),
			systems_.renderer()->screen_size()));

	sge::renderer::scoped_block scoped(
		systems_.renderer());

	process_event(
		events::render());

	if (show_stats_)
		stats_.update_and_render();

	console_.render();
}

boost::program_options::variables_map const &
insula::machine::cli_variables() const
{
	return vm_;
}

insula::graphics::camera::object &
insula::machine::camera()
{
	return *camera_;
}

sge::systems::instance const &
insula::machine::systems() const
{
	return systems_;
}

insula::console::object &
insula::machine::console()
{
	return console_;
}

bool 
insula::machine::running() const
{
	return running_;
}

insula::machine::~machine() {}
