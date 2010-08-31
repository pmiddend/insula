#ifndef INSULA_GRAPHICS_SHADER_VARIABLE_HPP_INCLUDED
#define INSULA_GRAPHICS_SHADER_VARIABLE_HPP_INCLUDED

#include "variable_type.hpp"
#include "value_type.hpp"
#include "value_type_impl.hpp"
#include "sampler_fwd.hpp"
#include <sge/renderer/glsl/string.hpp>

namespace insula
{
namespace graphics
{
namespace shader
{
class variable
{
public:
	explicit
	variable(
		sge::renderer::glsl::string const &name,
		variable_type::type,
		value_type const &);

	sge::renderer::glsl::string const
	declaration() const;

	variable_type::type
	type() const;

	sge::renderer::glsl::string const
	name() const;

	value_type const
	initial_value() const;
private:
	sge::renderer::glsl::string name_,declaration_;
	variable_type::type type_;
	value_type initial_value_;
};
}
}
}

#endif
