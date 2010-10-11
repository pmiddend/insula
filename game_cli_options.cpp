#include "game_cli_options.hpp"
#include "height_map/flatness_range.hpp"
#include <boost/program_options/value_semantic.hpp>
#include <fcppt/string.hpp>
#include <chrono>

boost::program_options::options_description const
insula::game_cli_options()
{
	boost::program_options::options_description opts("Game options");
	opts.add_options()
		("start-time",boost::program_options::value<std::chrono::milliseconds::rep>(),"The amount of time you have left at game start (in milliseconds)");
	return opts;
}
