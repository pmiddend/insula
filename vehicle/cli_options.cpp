#include "cli_options.hpp"
#include "../graphics/scalar.hpp"
#include <fcppt/string.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <vector>

boost::program_options::options_description const
insula::vehicle::cli_options()
{
	boost::program_options::options_description opts("Vehicle options");
	opts.add_options()
		("vehicle-camera-distance",boost::program_options::value<graphics::scalar>()->default_value(10),"Distance to the vehicle")
		("vehicle-camera-angle",boost::program_options::value<graphics::scalar>()->default_value(30),"Angle of the camera in degrees")
		// vehicle begin
		("vehicle-file",boost::program_options::value<fcppt::string>(),"The file to load the vehicle from");
	return opts;
}

