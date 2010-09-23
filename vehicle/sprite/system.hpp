#ifndef INSULA_VEHICLE_SPRITE_SYSTEM_HPP_HPP_INCLUDED
#define INSULA_VEHICLE_SPRITE_SYSTEM_HPP_HPP_INCLUDED

#include "choices.hpp"
#include <sge/sprite/system.hpp>
#include <sge/sprite/intrusive/system_impl.hpp>

namespace insula { namespace vehicle {
namespace sprite
{
typedef
sge::sprite::system
<
	choices
>::type
system;
}
}
}

#endif
