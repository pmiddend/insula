#ifndef INSULA_PHYSICS_APPROXIMATION_BOX_HPP_INCLUDED
#define INSULA_PHYSICS_APPROXIMATION_BOX_HPP_INCLUDED

#include "../dim3.hpp"
#include <fcppt/math/dim/basic_impl.hpp>

namespace insula
{
namespace physics
{
namespace approximation
{
struct box
{
	dim3 size;

	explicit
	box(
		dim3 const size) : size(size) {}
};
}
}
}

#endif
