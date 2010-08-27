#include "cli_options.hpp"
#include "../scalar.hpp"
#include <fcppt/math/twopi.hpp>
#include <boost/program_options/value_semantic.hpp>

boost::program_options::options_description const
insula::graphics::camera::cli_options()
{
	boost::program_options::options_description opts("Height map options");
	opts.add_options()
		("camera-roll-speed",boost::program_options::value<scalar>(),"Rolling speed of the camera")
		("camera-fov",boost::program_options::value<scalar>(),"Field of view (in degrees)")
		("camera-near",boost::program_options::value<scalar>(),"Distance to the near plane")
		("camera-far",boost::program_options::value<scalar>(),"Distance to the far plane")
		("camera-speed",boost::program_options::value<scalar>(),"Speed of the camera");
	return opts;
}

