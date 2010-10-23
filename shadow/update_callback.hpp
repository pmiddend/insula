#ifndef INSULA_SHADOW_UPDATE_CALLBACK_HPP_INCLUDED
#define INSULA_SHADOW_UPDATE_CALLBACK_HPP_INCLUDED

#include "update_callback_fn.hpp"
#include <functional>

namespace insula
{
namespace shadow
{
typedef
std::function<update_callback_fn>
update_callback;
}
}

#endif
