#ifndef INSULA_SCENE_RENDER_PASS_DEPENDENCY_SET_HPP_INCLUDED
#define INSULA_SCENE_RENDER_PASS_DEPENDENCY_SET_HPP_INCLUDED

#include "identifier.hpp"
#include <unordered_set>

namespace insula
{
namespace scene
{
namespace render_pass
{
typedef
std::unordered_set<identifier>
dependency_set;
}
}
}

#endif
