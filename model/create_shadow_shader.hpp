#ifndef INSULA_MODEL_CREATE_SHADOW_SHADER_HPP_INCLUDED
#define INSULA_MODEL_CREATE_SHADOW_SHADER_HPP_INCLUDED

#include "../graphics/shader/shared_object_ptr.hpp"
#include <sge/renderer/device_ptr.hpp>

namespace insula
{
namespace model
{
graphics::shader::shared_object_ptr const
create_shadow_shader(
	sge::renderer::device_ptr);
}
}

#endif
