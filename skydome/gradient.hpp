#ifndef INSULA_SKYDOME_GRADIENT_HPP_INCLUDED
#define INSULA_SKYDOME_GRADIENT_HPP_INCLUDED

#include "color.hpp"
#include <tuple>

namespace insula
{
namespace skydome
{
typedef
std::tuple
<
	color,
	color
>
gradient;
}
}

#endif
