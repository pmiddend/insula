#ifndef INSULA_MODEL_CREATE_SHADER_HPP_INCLUDED
#define INSULA_MODEL_CREATE_SHADER_HPP_INCLUDED

#include "../graphics/shader/shared_object_ptr.hpp"
#include "../graphics/vec3.hpp"
#include <sge/renderer/device_ptr.hpp>

namespace insula
{
namespace model
{
graphics::shader::shared_object_ptr const
create_shader(
	sge::renderer::device_ptr,
	graphics::vec3 const &light_source);
}
}

#endif
