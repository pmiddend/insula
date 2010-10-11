#include "sound_controller.hpp"
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
#include <sge/parse/json/array.hpp>
#include <sge/time/timer.hpp>
#include <sge/time/second.hpp>
#include <sge/audio/vector.hpp>
#include <fcppt/text.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/io/istringstream.hpp>
#include <fcppt/exception.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
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
		"    \"events\" : { \"skid\" : \"skid.wav\", \"motor\" : \"motor.wav\"  }\n"
		"  }\n"
		"}\n"));

	fcppt::io::cout 
		<< FCPPT_TEXT("File looks like \n") 
		<< ss.str() 
		<< FCPPT_TEXT("\n");

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
				{FCPPT_TEXT("wav"),FCPPT_TEXT("ogg")}))
		(sge::systems::audio_player_default()));

	insula::sound_controller music(
		sge::parse::json::find_member_exn<sge::parse::json::object>(
			o.members,
			FCPPT_TEXT("sounds")),
		sys.audio_loader(),
		sys.audio_player());

	fcppt::io::cout << FCPPT_TEXT("Ok, playing the nonpositional event sound \"motor\"\n");

	music.play(
		FCPPT_TEXT("motor"));

	sge::time::timer end_timer(
		sge::time::second(
			10));

	sge::time::timer play_timer(
		sge::time::second(
			2));

	bool played_sound = false;

	while (true)
	{
		if (end_timer.expired())
			break;

		if (play_timer.expired() && !played_sound)
		{
			fcppt::io::cout << FCPPT_TEXT("Playing sound \"skid\", positional\n");
			music.play_positional(
				FCPPT_TEXT("skid"),
				sge::audio::sound::positional_parameters()
					.position(
						sge::audio::vector(-3,0,0)));
			played_sound = true;
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
