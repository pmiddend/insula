#ifndef INSULA_GRAPHICS_SHADER_VECTOR_TO_STRING_HPP_INCLUDED
#define INSULA_GRAPHICS_SHADER_VECTOR_TO_STRING_HPP_INCLUDED

#include <sge/renderer/vf/vertex_size.hpp>
#include <sge/renderer/glsl/string.hpp>
#include <fcppt/lexical_cast.hpp>

namespace insula
{
namespace graphics
{
namespace shader
{
template<typename T,sge::renderer::vf::vertex_size N>
sge::renderer::glsl::string const
vector_to_string()
{
	return "vec"+fcppt::lexical_cast<sge::renderer::glsl::string>(N);
}
}
}
}

#endif
