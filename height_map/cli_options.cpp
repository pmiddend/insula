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
		("terrain-cell-size",boost::program_options::value<graphics::scalar>()->default_value(static_cast<graphics::scalar>(5.0)),"Size dimensions of a single grid cell (it will be quadratic, hence only one value)")
		("terrain-height-scaling",boost::program_options::value<graphics::scalar>()->default_value(static_cast<graphics::scalar>(250)),"Height scaling")
		("terrain-height-map",boost::program_options::value<fcppt::string>()->default_value(FCPPT_TEXT("test_256.png")),"Height map (has to be greyscale)")
		("terrain-gradient-texture",boost::program_options::value<fcppt::string>()->default_value(FCPPT_TEXT("rock_512.png")),"Texture for the gradient")
		("terrain-height-texture-1",boost::program_options::value<fcppt::string>()->default_value(FCPPT_TEXT("grass_512.png")),"Lower height texture")
		("terrain-height-texture-2",boost::program_options::value<fcppt::string>()->default_value(FCPPT_TEXT("sand_512.png")),"Upper height texture")
		("terrain-ambient-light",boost::program_options::value<graphics::scalar>()->default_value(static_cast<graphics::scalar>(0.4)),"Ambient lighting (in [0,1])")
		("terrain-sun-direction",boost::program_options::value<graphics::vec3>()->default_value(graphics::vec3(100,100,100)),"Sun direction (doesn't have to be normalized)")
		("terrain-texture-scaling",boost::program_options::value<graphics::scalar>()->default_value(static_cast<graphics::scalar>(20)),"Texture scaling; the higher the value, the more often the texture is repeating. A value of 1 means it's not repeating.");
	return opts;
}

