#include "create_variables_map.hpp"
#include "graphics/camera/cli_options.hpp"
#include "game_cli_options.hpp"
#include "graphics/cli_options.hpp"
#include "water/cli_options.hpp"
#include "states/camera_move.hpp"
#include "vehicle/cli_options.hpp"
#include "skydome/cli_options.hpp"
#include "height_map/cli_options.hpp"
#include "physics/cli_options.hpp"
#include "help_needed.hpp"
#include "media_path.hpp"
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/filesystem/exists.hpp>
#include <sstream>

namespace
{
fcppt::string const
options_to_string(
	boost::program_options::options_description const &o)
{
	std::ostringstream ss;
	ss << o;
	return 
		fcppt::from_std_string(
			ss.str());
}
}

boost::program_options::variables_map const
insula::create_variables_map(
	int argc,
	char **argv)
{
	boost::program_options::options_description desc("Allowed options");

	desc.add_options()
		("help","produce help message");

	desc.add(
		game_cli_options());

	desc.add(
		graphics::camera::cli_options());

	desc.add(
		height_map::cli_options());

	desc.add(
		water::cli_options());

	desc.add(
		skydome::cli_options());

	desc.add(
		vehicle::cli_options());

	desc.add(
		graphics::cli_options());

	desc.add(
		physics::cli_options());

	desc.add(
		states::camera_move::cli_options());

	boost::program_options::variables_map vm;

	boost::program_options::store(
		// Yes, the <char> is indented.
		boost::program_options::parse_config_file<char>(
			// NOTE: This is of course broken when the value_type of path
			// is not char*
			(media_path()/FCPPT_TEXT("config.po")).string().c_str(),
			desc), 
		vm);

	// I don't know if that's neccessary since we do it again after
	// parsing the command line
	boost::program_options::notify(
		vm);    

	if (fcppt::filesystem::exists(media_path()/FCPPT_TEXT("user_config.po")))
	{
		boost::program_options::store(
			// Yes, the <char> is indented.
			boost::program_options::parse_config_file<char>(
				// NOTE: This is of course broken when the value_type of path
				// is not char*
				(media_path()/FCPPT_TEXT("user_config.po")).string().c_str(),
				desc), 
			vm);

		// I don't know if that's neccessary since we do it again after
		// parsing the command line
		boost::program_options::notify(
			vm);    
	}
		

	boost::program_options::store(
		boost::program_options::parse_command_line(
			argc, 
			argv, 
			desc), 
		vm);

	boost::program_options::notify(
		vm);    

	if (vm.count("help")) 
		throw 
			help_needed(
				options_to_string(
					desc));

	return vm;
}
