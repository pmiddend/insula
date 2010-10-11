#include "cli_options.hpp"
#include "vec3.hpp"
#include <vector>

boost::program_options::options_description const
insula::physics::cli_options()
{
	boost::program_options::options_description opts("Physics options");
	opts.add_options()
		("physics-gravity",boost::program_options::value<vec3>(),"The gravity");
	return opts;
}

