#ifndef INSULA_VEHICLE_SPRITE_PARAMETERS_HPP_HPP_INCLUDED
#define INSULA_VEHICLE_SPRITE_PARAMETERS_HPP_HPP_INCLUDED

#include "choices.hpp"
#include <sge/sprite/parameters_decl.hpp>
#include <sge/sprite/parameters_impl.hpp>

namespace insula { namespace vehicle {
namespace sprite
{
typedef
sge::sprite::parameters
<
	choices
>
parameters;
}
}
}

#endif
