#ifndef INSULA_GRAPHICS_CAMERA_SCOPED_HPP_INCLUDED
#define INSULA_GRAPHICS_CAMERA_SCOPED_HPP_INCLUDED

#include "object_fwd.hpp"
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
		insula::graphics::gizmo const &new_gizmo);

	~scoped();
private:
	object &camera_;
	insula::graphics::gizmo const gizmo_;
};
}
}
}

#endif
