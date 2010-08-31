#ifndef INSULA_GRAPHICS_SHADER_VF_TO_STRING_HPP_INCLUDED
#define INSULA_GRAPHICS_SHADER_VF_TO_STRING_HPP_INCLUDED

#include "format_printer.hpp"
#include <sge/renderer/glsl/string.hpp>
#include <boost/mpl/for_each.hpp>

namespace insula
{
namespace graphics
{
namespace shader
{
template<typename Format>
sge::renderer::glsl::string const
vf_to_string()
{
	fcppt::string s;
	boost::mpl::for_each<typename Format::elements>(
		format_printer(
			s));
	return s;
}
}
}
}

#endif
