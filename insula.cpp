#include "scoped_machine.hpp"
#include "machine.hpp"
// The first state to be used has to be included here
#include "states/freelook.hpp"
#include "json/config_wrapper.hpp"
#include "media_path.hpp"
#include <sge/time/timer.hpp>
#include <sge/time/second.hpp>
#include <sge/parse/json/object.hpp>
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
	sge::parse::json::object const config_file =
		insula::json::config_wrapper(
			{"user_config.json"},
			argc,
			argv);

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
