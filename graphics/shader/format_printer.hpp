#ifndef INSULA_GRAPHICS_SHADER_FORMAT_PRINTER_HPP_INCLUDED
#define INSULA_GRAPHICS_SHADER_FORMAT_PRINTER_HPP_INCLUDED

#include "vector_to_string.hpp"
#include <sge/renderer/glsl/string.hpp>
#include <sge/renderer/vf/vertex_size.hpp>
#include <sge/renderer/vf/unspecified.hpp>
#include <sge/renderer/vf/vector.hpp>
#include <boost/format.hpp>

namespace insula
{
namespace graphics
{
namespace shader
{
// It used in a for_each to iterate over all components of a format
// and store the result in a string
class format_printer
{
public:
	explicit 
	format_printer(
		sge::renderer::glsl::string &s) 
	: s(s) 
	{}

	template
	<
		typename T,
		typename Tag,
		sge::renderer::vf::vertex_size N
	>
	void
	operator()(
		sge::renderer::vf::unspecified
		<
			sge::renderer::vf::vector<T,N>,Tag>)
	{
		s += 
			boost::str(
				boost::format("in %s %s;\n")
				% vector_to_string<T,N>()
				% Tag::name());
	}

	template<typename T>
	void
	operator()(T const &t) const
	{
	}
private:
	fcppt::string &s;
};
}
}
}

#endif
