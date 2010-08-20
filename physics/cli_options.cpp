#include "cli_options.hpp"
#include "vec3.hpp"
#include <fcppt/math/vector/input.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <vector>

boost::program_options::options_description const
insula::physics::cli_options()
{
	boost::program_options::options_description opts("Physics options");
	opts.add_options()
		("physics-gravity",boost::program_options::value<vec3>()->default_value(physics::vec3(0,-10,0)),"The gravity");
	return opts;
}

