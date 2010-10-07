#ifndef INSULA_SCENE_RENDER_PASS_CALLBACKS_VIEWPORT_HPP_INCLUDED
#define INSULA_SCENE_RENDER_PASS_CALLBACKS_VIEWPORT_HPP_INCLUDED

#include <sge/renderer/viewport.hpp>
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
std::function<sge::renderer::viewport()>
viewport;
}
}
}
}

#endif
