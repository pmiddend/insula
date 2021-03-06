#ifndef INSULA_SCENE_RENDER_PASS_CALLBACKS_CAMERA_HPP_INCLUDED
#define INSULA_SCENE_RENDER_PASS_CALLBACKS_CAMERA_HPP_INCLUDED

#include "../../../graphics/gizmo.hpp"
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
std::function<graphics::gizmo()>
camera;
}
}
}
}

#endif
