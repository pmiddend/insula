#ifndef INSULA_SCENE_RENDER_PASS_CALLBACK_HPP_INCLUDED
#define INSULA_SCENE_RENDER_PASS_CALLBACK_HPP_INCLUDED

#include "manager_fwd.hpp"
#include <functional>

namespace insula
{
namespace scene
{
typedef
std::function<void (manager &)>
render_pass_callback;
}
}

#endif
