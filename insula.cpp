#include "scoped_machine.hpp"
#include "machine.hpp"
// The first state to be used has to be included here
#include "states/freelook.hpp"
#include "json/process_option.hpp"
#include "json/merge_trees.hpp"
#include "media_path.hpp"
#include <sge/time/timer.hpp>
#include <sge/time/second.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/parse_file_exn.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/text.hpp>
#include <fcppt/from_std_string.hpp>
#include <iostream>
#include <cstddef>
#include <cstdlib>

int main(int argc,char *argv[])
try
{
	sge::parse::json::object config_file =
		insula::json::merge_trees(
			sge::parse::json::parse_file_exn(
				insula::media_path()/FCPPT_TEXT("config.json")),
			sge::parse::json::parse_file_exn(
				insula::media_path()/FCPPT_TEXT("user_config.json")));

	if (argc >= 2 && std::string(argv[1]) == "--help")
	{
		std::cout << 
			"Command line options are of the form:\n\n"
			"foo/bar/baz=qux\n\n"
			"where foo/bar/baz is a sequence of objects in the config.json file.\n"
			"qux can be any json type (strings, arrays, etc.)\n"
			"For example, if config.json looks like this:\n\n"
			"{ \"renderer\" : { \"screen_size\" : [640,480] } }\n\n"
			"You could change the resolution via:\n\n"
			"renderer/screen_size=[1024,768]\n\n"
			"Be aware of two things, though:\n"
			"1. You can use _any_ json type, so it's possible to write\n\n"
			"renderer/screen_size=1.0\n\n"
			"which, of course, makes no sense. Try to use the correct type.\n"
			"2. Be aware of your shell' special characters. For example, in bash\n"
			"to set a json string, you have to write:\n\n"
			"player/name='\"foobar\"'\n\n"
			"It's a good idea to always put the argument in apostrophes.\n";
		return EXIT_SUCCESS;
	}

	for (int i = 1; i < argc; ++i)
		insula::json::process_option(
			config_file,
			fcppt::from_std_string(
				argv[i]));

	insula::machine m(
		config_file);

	insula::scoped_machine scope(
		m);

	sge::time::timer frame_timer(
		sge::time::second(1));
	while (m.running())
		m.tick(
			frame_timer.reset());
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
