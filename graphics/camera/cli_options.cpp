#include "cli_options.hpp"
#include "../scalar.hpp"
#include <fcppt/math/twopi.hpp>
#include <boost/program_options/value_semantic.hpp>

boost::program_options::options_description const
insula::graphics::camera::cli_options()
{
	boost::program_options::options_description opts("Height map options");
	opts.add_options()
		("camera-roll-speed",boost::program_options::value<scalar>()->default_value(fcppt::math::twopi<graphics::scalar>()/8),"Rolling speed of the camera")
		("camera-fov",boost::program_options::value<scalar>()->default_value(90),"Field of view (in degrees)")
		("camera-near",boost::program_options::value<scalar>()->default_value(1.0),"Distance to the near plane")
		("camera-far",boost::program_options::value<scalar>()->default_value(10000),"Distance to the far plane")
		("camera-speed",boost::program_options::value<scalar>()->default_value(500),"Speed of the camera");
	return opts;
}

