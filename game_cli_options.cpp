#include "game_cli_options.hpp"
#include "nugget_sequence.hpp"
#include <boost/program_options/value_semantic.hpp>

boost::program_options::options_description const
insula::game_cli_options()
{
	boost::program_options::options_description opts("Game options");
	opts.add_options()
		("game-nuggets",boost::program_options::value<nugget_sequence::size_type>()->default_value(10),"The number of nuggets to place randomly");
	return opts;
}
