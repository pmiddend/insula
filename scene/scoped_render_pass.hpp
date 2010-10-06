#ifndef INSULA_SCENE_SCOPED_RENDER_PASS_HPP_INCLUDED
#define INSULA_SCENE_SCOPED_RENDER_PASS_HPP_INCLUDED

#include "render_pass_impl_fwd.hpp"
#include <memory>

namespace insula
{
namespace scene
{
typedef
std::shared_ptr<render_pass_impl>
scoped_render_pass;
}
}

#endif
