#include "cli_options.hpp"
#include "size_type.hpp"
#include "../graphics/vec3.hpp"
#include "../graphics/scalar.hpp"
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/math/vector/input.hpp>
#include <fcppt/string.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <vector>

boost::program_options::options_description const
insula::skydome::cli_options()
{
	boost::program_options::options_description opts("Skydome options");
	opts.add_options()
		("skydome-latitudes",boost::program_options::value<skydome::size_type>(),"How many latitude iterations")
		("skydome-longitudes",boost::program_options::value<skydome::size_type>(),"How many longitude iterations")
		("skydome-angle",boost::program_options::value<graphics::scalar>(),"Total angle (in degrees); currently unused")
		("skydome-color0",boost::program_options::value<graphics::vec3>(),"The skydome's base color. As its name suggests, this color will be shown at the base of the dome.")
		("skydome-color1",boost::program_options::value<graphics::vec3>(),"The skydome's top color. This will be shown at the top of the dome.");
	return opts;
}

