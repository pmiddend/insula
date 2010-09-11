#ifndef INSULA_PHYSICS_APPROXIMATION_STRING_TO_CYLINDER_ORIENTATION_HPP_INCLUDED
#define INSULA_PHYSICS_APPROXIMATION_STRING_TO_CYLINDER_ORIENTATION_HPP_INCLUDED

#include "cylinder_orientation.hpp"
#include "../../exception.hpp"
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>

namespace insula
{
namespace physics
{
namespace approximation
{
inline cylinder_orientation::type
string_to_cylinder_orientation(
	fcppt::string const &x)
{
	if (x == FCPPT_TEXT("x"))
		return cylinder_orientation::x;
	if (x == FCPPT_TEXT("y"))
		return cylinder_orientation::y;
	if (x == FCPPT_TEXT("z"))
		return cylinder_orientation::z;
	throw exception(FCPPT_TEXT("Valid cylinder orientations are: x, y and z"));
}
}
}
}

#endif
