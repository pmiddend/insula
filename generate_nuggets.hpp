#ifndef INSULA_GENERATE_NUGGETS_HPP_INCLUDED
#define INSULA_GENERATE_NUGGETS_HPP_INCLUDED

#include "nugget_sequence.hpp" 
#include "height_map/object_fwd.hpp"
#include "height_map/flatness_range.hpp"
#include "graphics/scalar.hpp"

namespace insula
{
// Uses random_flat_point to randomly generate "count" nuggets on the
// given terrain
nugget_sequence const
generate_nuggets(
	height_map::object const &,
	graphics::scalar water_level,
	nugget_sequence::size_type count,
	height_map::flatness_range const &);
}

#endif
