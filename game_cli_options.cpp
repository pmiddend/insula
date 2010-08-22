#include "game_cli_options.hpp"
#include "nugget_sequence.hpp"
#include <boost/program_options/value_semantic.hpp>
#include <fcppt/text.hpp>
#include <fcppt/string.hpp>

boost::program_options::options_description const
insula::game_cli_options()
{
	boost::program_options::options_description opts("Game options");
	opts.add_options()
		("game-nuggets",boost::program_options::value<nugget_sequence::size_type>()->default_value(10),"The number of nuggets to place randomly")
		("game-nugget-model",boost::program_options::value<fcppt::string>()->default_value(FCPPT_TEXT("props/beacon.md3")),"The nugget model")
		("game-nugget-texture",boost::program_options::value<fcppt::string>()->default_value(FCPPT_TEXT("props/beacon.png")),"The nugget texture");
	return opts;
}
