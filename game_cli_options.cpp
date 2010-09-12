#include "game_cli_options.hpp"
#include "nugget_sequence.hpp"
#include "height_map/flatness_range.hpp"
#include <boost/program_options/value_semantic.hpp>
#include <fcppt/math/vector/input.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/string.hpp>

boost::program_options::options_description const
insula::game_cli_options()
{
	boost::program_options::options_description opts("Game options");
	opts.add_options()
		("game-nuggets",boost::program_options::value<nugget_sequence::size_type>(),"The number of nuggets to place randomly")
		("game-nugget-model",boost::program_options::value<fcppt::string>(),"The nugget model")
		("game-nugget-flatness-range",boost::program_options::value<height_map::flatness_range>(),"In which flatness range the nuggets shall be generated. It's a vector in [0,1]")
		("game-nugget-texture",boost::program_options::value<fcppt::string>(),"The nugget texture");
	return opts;
}
