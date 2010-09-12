#include "cli_options.hpp"
#include "../graphics/scalar.hpp"
#include "../height_map/flatness_range.hpp"
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/vector/input.hpp>
#include <fcppt/math/vector/output.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <vector>

boost::program_options::options_description const
insula::vehicle::cli_options()
{
	boost::program_options::options_description opts("Vehicle options");
	opts.add_options()
		("vehicle-camera-distance",boost::program_options::value<graphics::scalar>(),"Distance to the vehicle")
		("vehicle-camera-angle",boost::program_options::value<graphics::scalar>(),"Angle of the camera in degrees")
		// vehicle begin
		("vehicle-file",boost::program_options::value<fcppt::string>(),"The file to load the vehicle from")
		("vehicle-flatness-range",boost::program_options::value<height_map::flatness_range>(),"A vector in [0,1] specifying the gradient range in which the vehicle shall be generated");
	return opts;
}

