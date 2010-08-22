#ifndef INSULA_NUGGET_SEQUENCE_HPP_INCLUDED
#define INSULA_NUGGET_SEQUENCE_HPP_INCLUDED

#include "graphics/vec2.hpp"
#include <fcppt/math/vector/basic_impl.hpp>
#include <vector>

namespace insula
{
// Those are the positions from which the model list is
// reconstructed each turn. We only need the x and z coordinates,
// the y coordinates are determined from that
typedef
std::vector<graphics::vec2>
nugget_sequence;
}

#endif
