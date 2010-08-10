#ifndef INSULA_GRAPHICS_CAMERA_SCOPED_HPP_INCLUDED
#define INSULA_GRAPHICS_CAMERA_SCOPED_HPP_INCLUDED

#include "object_fwd.hpp"
#include "../vec3.hpp"
#include "../gizmo.hpp"
#include <fcppt/math/vector/basic_impl.hpp>

namespace insula
{
namespace graphics
{
namespace camera
{
class scoped
{
public:
	scoped(scoped const &) = delete;
	scoped&operator=(scoped const &) = delete;
	
	explicit
	scoped(
		object &_camera,
		vec3 const &new_position,
		gizmo const &new_gizmo);

	~scoped();
private:
	object &camera_;
	vec3 const position_;
	gizmo const gizmo_;
};
}
}
}

#endif
