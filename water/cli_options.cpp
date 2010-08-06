#include "cli_options.hpp"
#include "../graphics/scalar.hpp"
#include <sge/renderer/dim_type.hpp>
#include <fcppt/math/dim/input.hpp>
#include <fcppt/math/dim/output.hpp>
#include <fcppt/string.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <vector>

boost::program_options::options_description const
insula::water::cli_options()
{
	boost::program_options::options_description opts("Water options");
	opts.add_options()
		("water-level",boost::program_options::value<graphics::scalar>()->default_value(static_cast<graphics::scalar>(5)),"Water level")
		("water-reflection-size",boost::program_options::value<sge::renderer::dim_type>()->default_value(sge::renderer::dim_type(512,512)),"Size of the water reflection texture. If it is equivalent to the screen size")
		("water-bump-map",boost::program_options::value<fcppt::string>()->default_value(FCPPT_TEXT("bumps.png")),"Location of the water bump map. This is a file below the media path and it has to be an rgb file.")
		("water-bump-scaling",boost::program_options::value<graphics::scalar>()->default_value(static_cast<graphics::scalar>(20)),"The texture scaling for the bump texture")
		("water-wave-height",boost::program_options::value<graphics::scalar>()->default_value(static_cast<graphics::scalar>(0.5)),"The amount of wave perturbation (the \"height\" of the waves)")
		("water-wind-speed",boost::program_options::value<graphics::scalar>()->default_value(static_cast<graphics::scalar>(0.125)),"The speed of the water.");
	return opts;
}
