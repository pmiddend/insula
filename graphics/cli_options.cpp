#include "cli_options.hpp"
#include <sge/renderer/screen_size.hpp>
#include <fcppt/math/dim/basic_impl.hpp>
#include <fcppt/math/dim/input.hpp>
#include <fcppt/math/dim/output.hpp>
#include <boost/program_options/value_semantic.hpp>

boost::program_options::options_description const
insula::graphics::cli_options()
{
	boost::program_options::options_description opts("Renderer options");
	opts.add_options()
		("graphics-screen-size",boost::program_options::value<sge::renderer::screen_size>(),"The size of the screen");
	return opts;
}

