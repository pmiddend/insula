#include <sge/parse/json/array.hpp>
#include "music_controller.hpp"
#include <sge/exception.hpp>
#include <sge/systems/audio_loader.hpp>
#include <sge/systems/audio_player_default.hpp>
#include <sge/audio/loader_capabilities_field.hpp>
#include <sge/audio/loader.hpp>
#include <sge/extension_set.hpp>
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/parse_stream.hpp>
#include <sge/parse/json/find_member_exn.hpp>
#include <sge/time/timer.hpp>
#include <sge/time/second.hpp>
#include <fcppt/text.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/io/istringstream.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/from_std_string.hpp>
#include <exception>
#include <iostream>
#include <ostream>
#include <cstddef>

int main()
try
{
	fcppt::io::istringstream ss(
		fcppt::from_std_string(
		"{\n"
		"  \"sounds\" : {\n"
		"    \"events\" : { \"bang\" : \"/tmp/bang.ogg\" },\n"
		"    \"random\" : [ \"/tmp/hit.ogg\" ],\n"
		"    \"crossfade_seconds\" : 5\n"
		"  }\n"
		"}\n"));

	fcppt::io::cout << FCPPT_TEXT("File looks like \n") << ss.str() << FCPPT_TEXT("\n");

	sge::parse::json::object o;

	if (!sge::parse::json::parse_stream(ss,o))
	{
		fcppt::io::cerr << FCPPT_TEXT("parse_stream failed\n");
		return EXIT_FAILURE;
	}

	sge::systems::instance sys(
		sge::systems::list() 
		(
			sge::systems::audio_loader(
				sge::audio::loader_capabilities_field::null(),
				fcppt::assign::make_container<sge::extension_set>
					(FCPPT_TEXT("wav"))
					(FCPPT_TEXT("ogg"))))
		(sge::systems::audio_player_default()));

	insula::music_controller music(
		sge::parse::json::find_member_exn<sge::parse::json::object>(
			o.members,
			FCPPT_TEXT("sounds")),
		sys.audio_loader(),
		sys.audio_player());

	fcppt::io::cout << FCPPT_TEXT("Ok, playing the event sound \"bang\"\n");

	music.play_event(
		FCPPT_TEXT("bang"));

	sge::time::timer change_timer(
		sge::time::second(
			10));

	bool second_sound = false;

	sge::time::timer stop_timer(
		sge::time::second(
			20));

	bool stopped = false;

	while (true)
	{
		music.update();

		if (change_timer.expired() && !second_sound)
		{
			fcppt::io::cout << FCPPT_TEXT("Now changing the sound\n");
			music.play_random();
			second_sound = true;
		}

		if (stop_timer.expired() && !stopped)
		{
			fcppt::io::cout << FCPPT_TEXT("Now stopping the sound\n");
			music.stop();
			stopped = true;
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
	std::cerr << e.what() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}
