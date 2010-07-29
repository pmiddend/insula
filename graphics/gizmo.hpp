#ifndef INSULA_GRAPHICS_GIZMO_HPP_INCLUDED
#define INSULA_GRAPHICS_GIZMO_HPP_INCLUDED

#include "vec3.hpp"
#include "gizmo_init.hpp"
#include <fcppt/math/vector/basic_impl.hpp>

namespace insula
{
namespace graphics
{
class gizmo
{
public:
	// NOTE: One of the few places where explicit is not really needed
	gizmo(
		gizmo_init const &);

	gizmo &operator=(
		gizmo_init const &);

	vec3 const &
	forward() const;

	vec3 const &
	right() const;

	vec3 const &
	up() const;
private:
	vec3 forward_,right_,up_;
};
}
}

#endif
