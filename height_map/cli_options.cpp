#include "cli_options.hpp"
#include "../graphics/vec2.hpp"
#include "../graphics/vec3.hpp"
#include "../graphics/scalar.hpp"
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/math/vector/input.hpp>
#include <fcppt/string.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <vector>

boost::program_options::options_description const
insula::height_map::cli_options()
{
	boost::program_options::options_description opts("Height map options");
	opts.add_options()
		("terrain-cell-size",boost::program_options::value<graphics::scalar>(),"Size dimensions of a single grid cell (it will be quadratic, hence only one value)")
		("terrain-height-scaling",boost::program_options::value<graphics::scalar>(),"Height scaling")
		("terrain-height-map",boost::program_options::value<fcppt::string>(),"Height map (has to be greyscale)")
		("terrain-gradient-texture",boost::program_options::value<fcppt::string>(),"Texture for the gradient")
		("terrain-height-texture-1",boost::program_options::value<fcppt::string>(),"Lower height texture")
		("terrain-height-texture-2",boost::program_options::value<fcppt::string>(),"Upper height texture")
		("terrain-ambient-light",boost::program_options::value<graphics::scalar>(),"Ambient lighting (in [0,1])")
		("terrain-sun-direction",boost::program_options::value<graphics::vec3>(),"Sun direction (doesn't have to be normalized)")
		("terrain-texture-scaling",boost::program_options::value<graphics::scalar>(),"Texture scaling; the higher the value, the more often the texture is repeating. A value of 1 means it's not repeating.");
	return opts;
}

