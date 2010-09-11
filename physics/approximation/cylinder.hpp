#ifndef INSULA_PHYSICS_APPROXIMATION_CYLINDER_HPP_INCLUDED
#define INSULA_PHYSICS_APPROXIMATION_CYLINDER_HPP_INCLUDED

#include "cylinder_orientation.hpp"
#include "../scalar.hpp"

namespace insula
{
namespace physics
{
namespace approximation
{
class cylinder
{
public:
	explicit
	cylinder(
		cylinder_orientation::type const orientation,
		scalar const height,
		scalar const radius)
	:
		orientation(orientation),
		height(height),
		radius(radius)
	{
	}

	cylinder_orientation::type orientation;
	scalar height;
	scalar radius;
};
}
}
}

#endif
