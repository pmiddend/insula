#include "machine.hpp"
#include "application_title.hpp"
#include "graphics/camera/parameters.hpp"
// Camera has to be updated in tick
#include "graphics/camera/object.hpp"
#include "graphics/scalar.hpp"
#include "graphics/vec3.hpp"
#include "json/find_member.hpp"
#include "events/tick.hpp"
#include "events/key.hpp"
#include "events/mouse_axis.hpp"
#include "events/mouse_button.hpp"
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
#include <sge/input/keyboard/collector.hpp>
#include <sge/input/mouse/collector.hpp>
#include <sge/input/mouse/axis_event_fwd.hpp>
#include <sge/input/mouse/button_event_fwd.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/key_event_fwd.hpp>
// For the exit callback end
#include <sge/systems/parameterless.hpp>
#include <sge/systems/image_loader.hpp>
#include <sge/systems/input.hpp>
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
#include <fcppt/math/deg_to_rad.hpp>
// Viewport hack begin
#include <sge/renderer/viewport.hpp>
#include <sge/renderer/pixel_pos.hpp>
// Viewport hack end
#include <sge/mainloop/dispatch.hpp>
#include <functional>

insula::machine::machine(
	sge::parse::json::object const &_config_file)
:
	// Has to be initialized early because the exit callback needs it
	running_(true),
	config_file_(
		_config_file),
	systems_(
		sge::systems::list
		(
			sge::window::parameters(
				application_title()))
		(
			sge::systems::audio_loader(
				sge::audio::loader_capabilities_field::null(),
				{FCPPT_TEXT("wav"),FCPPT_TEXT("ogg")}))
		(sge::systems::audio_player_default())
		(
			sge::renderer::parameters(
				sge::renderer::display_mode(
					// hier get_option durch json::find_member ersetzen, danach
					// weitermachen, boost::po zu entfernen
					json::find_member<sge::renderer::screen_size>(
						config_file_,
						FCPPT_TEXT("graphics/screen-size")),
					sge::renderer::bit_depth::depth32,
					sge::renderer::refresh_rate_dont_care),
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
				sge::all_extensions))),
	console_(
		systems_.keyboard_collector(),
		systems_.renderer(),
		systems_.font_system(),
		systems_.image_loader(),
		json::find_member<bool>(
			config_file_,
			"console/redirect") 
			? 
				console::redirect_mode::all
			: 
				console::redirect_mode::none),
	input_delegator_(
		systems_,
		console_),
	camera_(
		graphics::camera::parameters(
			input_delegator_,
			sge::renderer::aspect<graphics::scalar>(
				systems_.renderer()->screen_size()),
			fcppt::math::deg_to_rad(
				json::find_member<graphics::scalar>(
					config_file_,
					FCPPT_TEXT("camera/fov"))),
			json::find_member<graphics::scalar>(
				config_file_,
				FCPPT_TEXT("camera/near")),
			json::find_member<graphics::scalar>(
				config_file_,
				FCPPT_TEXT("camera/far")),
			json::find_member<graphics::scalar>(
				config_file_,
				FCPPT_TEXT("camera/movement-speed")),
			json::find_member<graphics::scalar>(
				config_file_,
				FCPPT_TEXT("camera/rotation-speed")),
			graphics::vec3::null())),
	exit_callback_(
		systems_.keyboard_collector()->key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::escape,
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
	key_callback_(
		systems_.keyboard_collector()->key_callback(
			[this](sge::input::keyboard::key_event const &e) 
			{
				this->process_event(
					events::key(
						e));
			})),
	mouse_axis_callback_(
		systems_.mouse_collector()->axis_callback(
			[this](sge::input::mouse::axis_event const &e) 
			{
				this->process_event(
					events::mouse_axis(
						e));
			})),
	mouse_button_callback_(
		systems_.mouse_collector()->button_callback(
			[this](sge::input::mouse::button_event const &e) 
			{
				this->process_event(
					events::mouse_button(
						e));
			})),
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

	camera_.update(
		time_delta);

	process_event(
		events::tick(
			time_delta));

	music_.update();
	sounds_.update();

	process_event(
		events::render());

	/*
	if (show_stats_)
		stats_.update_and_render();

	console_.render();
	*/
}

insula::graphics::camera::object &
insula::machine::camera()
{
	return camera_;
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

sge::parse::json::object const &
insula::machine::config_file() const
{
	return config_file_;
}

insula::music_controller &
insula::machine::music()
{
	return music_;
}

insula::sound_controller &
insula::machine::sounds()
{
	return sounds_;
}

insula::input_delegator &
insula::machine::input_delegator()
{
	return input_delegator_;
}

bool 
insula::machine::running() const
{
	return running_;
}

insula::machine::~machine() {}
