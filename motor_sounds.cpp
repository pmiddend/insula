#include "media_path.hpp"
#include <sge/log/global.hpp>
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/systems/audio_loader.hpp>
#include <sge/systems/parameterless.hpp>
#include <sge/audio/loader.hpp>
#include <sge/audio/player.hpp>
#include <sge/audio/multi_loader.hpp>
#include <sge/audio/buffer.hpp>
#include <sge/audio/sound/base.hpp>
#include <sge/audio/sound/repeat.hpp>
#include <sge/audio/loader_capabilities_field.hpp>
#include <sge/extension_set.hpp>
#include <sge/window/parameters.hpp>
#include <sge/window/dim_type.hpp>
#include <sge/window/dim_type.hpp>
#include <sge/input/system.hpp>
#include <sge/input/action.hpp>
#include <sge/audio/exception.hpp>
#include <sge/exception.hpp>
#include <sge/mainloop/dispatch.hpp>
#include <sge/window/instance.hpp>
#include <sge/time/timer.hpp>
#include <sge/time/second.hpp>
#include <sge/time/second_f.hpp>
#include <sge/input/key_pair.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/log/level.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/math/clamp.hpp>
#include <fcppt/text.hpp>
#include <exception>
#include <iostream>
#include <functional>
#include <ostream>
#include <algorithm>
#include <cstddef>

using namespace insula;

namespace
{
typedef float speed_type;
typedef unsigned gear_type;

speed_type const acceleration = 
	static_cast<speed_type>(5);
speed_type const max_speed = 
	static_cast<speed_type>(300);
speed_type const friction = 
	static_cast<speed_type>(1);

void
key_callback(
	sge::input::key_pair const &k,
	speed_type &current_acceleration)
{
	if (k.key().code() != sge::input::kc::key_up)
		return;
	current_acceleration = 
		k.value() ? acceleration :static_cast<speed_type>(0);
}

sge::audio::scalar
speed_to_pitch(
	speed_type const s)
{
	if (s <= static_cast<speed_type>(10))
		return 
			static_cast<sge::audio::scalar>(s) / 
			static_cast<sge::audio::scalar>(10);
	if (s <= static_cast<speed_type>(30))
		return 
			static_cast<sge::audio::scalar>(s - static_cast<sge::audio::scalar>(10)) / 
			static_cast<sge::audio::scalar>(30 - 10);
	if (s <= static_cast<speed_type>(50))
		return 
			static_cast<sge::audio::scalar>(s - static_cast<sge::audio::scalar>(30)) / 
			static_cast<sge::audio::scalar>(50 - 30);
	if (s <= static_cast<speed_type>(70))
		return 
			static_cast<sge::audio::scalar>(s - static_cast<sge::audio::scalar>(50)) / 
			static_cast<sge::audio::scalar>(70 - 50);
	return
		std::min(
			static_cast<sge::audio::scalar>(s - static_cast<sge::audio::scalar>(70)) / 
			static_cast<sge::audio::scalar>(20),
			static_cast<sge::audio::scalar>(1.0));
}
}

int main(int, char *[])
try
{
	fcppt::log::activate_levels(
		sge::log::global(),
		fcppt::log::level::debug);

	fcppt::filesystem::path const 
		file_name = 
			media_path() / 
			FCPPT_TEXT("sounds") /
			FCPPT_TEXT("motor.wav");

	sge::systems::instance sys(
		sge::systems::list()
		(sge::systems::parameterless::audio_player)
		(
			sge::systems::audio_loader(
				sge::audio::loader_capabilities_field::null(),
				fcppt::assign::make_container<sge::extension_set>
					(FCPPT_TEXT("wav"))))
		(sge::systems::parameterless::input)
		(sge::window::parameters(
				FCPPT_TEXT("insula motor sounds"))
			.dim(
				sge::window::dim_type(640, 480))));

	bool running = true;

	fcppt::signal::scoped_connection const cb(
		sys.input_system()->register_callback(
			sge::input::action(
				sge::input::kc::key_escape,
				[&running]() { running = false; })));

	speed_type 
		speed = 
			static_cast<speed_type>(0.0),
		delta = 
			static_cast<speed_type>(0),
		current_acceleration = 
			static_cast<speed_type>(0);

	fcppt::signal::scoped_connection const cb2(
		sys.input_system()->register_callback(
			std::bind(
				&key_callback,
				std::placeholders::_1,
				std::ref(current_acceleration))));

	sge::audio::buffer_ptr const buf = 
		sys.audio_player()->create_buffer(
			sys.audio_loader().load(
				file_name));

	sge::audio::sound::base_ptr const s = 
		buf->create_nonpositional();

	s->play(
		sge::audio::sound::repeat::loop);

	sys.window()->show();

	sge::time::timer frame_timer(
		sge::time::second(1));
	sge::time::timer display_timer(
		sge::time::second_f(static_cast<sge::time::funit>(0.1)));
	while (running)	
	{
		delta = 
			static_cast<speed_type>(
				frame_timer.reset());
		sge::mainloop::dispatch();
		if (display_timer.expired())
		{
			display_timer.reset();
			fcppt::io::cout << FCPPT_TEXT("\rspeed: ") << speed;
			fcppt::io::cout.flush();
		}
		speed = 
			fcppt::math::clamp(
				speed + (current_acceleration - friction) * delta,
				static_cast<speed_type>(
					0),
				max_speed);
		s->pitch(
			static_cast<sge::audio::scalar>(1.0) + 
			speed_to_pitch(
				speed));
	}

} catch (sge::audio::exception const &e) {
	fcppt::io::cerr << FCPPT_TEXT("audio exception caught: ") << e.string() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
} catch (sge::exception const &e) {
	fcppt::io::cerr << FCPPT_TEXT("Exception caught: ") << e.string() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
} catch (std::exception const &e) {
	std::cerr << "Exception caught: " << e.what() << '\n';
	return EXIT_FAILURE;
}
