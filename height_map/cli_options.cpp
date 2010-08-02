#include "cli_options.hpp"
#include "../graphics/vec2.hpp"
#include "../graphics/vec3.hpp"
#include "../graphics/scalar.hpp"
#include <boost/program_options/value_semantic.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/math/vector/input.hpp>
#include <fcppt/string.hpp>
#include <vector>

boost::program_options::options_description const
insula::height_map::cli_options()
{
	boost::program_options::options_description opts("Height map options");
	opts.add_options()
		("cell-size",boost::program_options::value<graphics::scalar>()->default_value(20),"Size dimensions of a single grid cell (it will be quadratic, hence only one value)")
		("height-scaling",boost::program_options::value<graphics::scalar>()->default_value(5000),"Height scaling")
		("height-map",boost::program_options::value<fcppt::string>()->required(),"Height map (has to be greyscale)")
		("gradient-texture",boost::program_options::value<fcppt::string>()->required(),"Texture for the gradient")
		("height-texture",boost::program_options::value<std::vector<fcppt::string>>()->multitoken(),"Height texture")
		("ambient-light",boost::program_options::value<graphics::scalar>()->default_value(static_cast<graphics::scalar>(0.4)),"Ambient lighting (in [0,1])")
		("sun-direction",boost::program_options::value<graphics::vec3>()->default_value(graphics::vec3(100,100,100)),"Sun direction (doesn't have to be normalized)")
		("texture-scaling",boost::program_options::value<graphics::scalar>()->default_value(static_cast<graphics::scalar>(20)),"Texture scaling; the higher the value, the more often the texture is repeating. A value of 1 means it's not repeating.");
	return opts;
}

