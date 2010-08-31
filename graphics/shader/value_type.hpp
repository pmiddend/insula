#ifndef INSULA_GRAPHICS_SHADER_VALUE_TYPE_HPP_INCLUDED
#define INSULA_GRAPHICS_SHADER_VALUE_TYPE_HPP_INCLUDED

#include "../vec2.hpp"
#include "../vec3.hpp"
#include "../vec4.hpp"
#include "../mat4.hpp"
#include "../scalar.hpp"
#include <fcppt/variant/object_decl.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace insula
{
namespace graphics
{
namespace shader
{
typedef
fcppt::variant::object
<
	boost::mpl::vector6
	<
		vec2,
		vec3,
		vec4,
		mat4,
		scalar,
		int
	>
>
value_type;
}
}
}

#endif
