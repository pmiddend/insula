#ifndef INSULA_SCENE_RENDER_PASS_CALLBACKS_TARGET_HPP_INCLUDED
#define INSULA_SCENE_RENDER_PASS_CALLBACKS_TARGET_HPP_INCLUDED

#include <sge/renderer/target_ptr.hpp>
#include <functional>

namespace insula
{
namespace scene
{
namespace render_pass
{
namespace callbacks
{
typedef
std::function<sge::renderer::target_ptr()>
target;
}
}
}
}

#endif
