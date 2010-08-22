#include "create_variables_map.hpp"
#include "machine.hpp"
// The first state to be used has to be included here
#include "states/freelook.hpp"
#include "help_needed.hpp"
#include <sge/time/timer.hpp>
#include <sge/time/second.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/io/cerr.hpp>
#include <iostream>
#include <cstddef>

int main(int argc,char *argv[])
try
{
	insula::machine m(
		insula::create_variables_map(
			argc,
			argv));

	m.initiate();

	sge::time::timer frame_timer(
		sge::time::second(1));
	while (m.running())
		m.tick(
			frame_timer.reset());
}
catch (insula::help_needed const &h)
{
	fcppt::io::cout << h.string() << FCPPT_TEXT("\n");
	return EXIT_SUCCESS;
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
