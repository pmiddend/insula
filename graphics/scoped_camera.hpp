#ifndef INSULA_GRAPHICS_SCOPED_CAMERA_HPP_INCLUDED
#define INSULA_GRAPHICS_SCOPED_CAMERA_HPP_INCLUDED

#include "camera_fwd.hpp"
#include "vec3.hpp"
#include "gizmo.hpp"
#include <fcppt/math/vector/basic_impl.hpp>

namespace insula
{
namespace graphics
{
class scoped_camera
{
public:
	scoped_camera(scoped_camera const &) = delete;
	scoped_camera &operator=(scoped_camera const &) = delete;
	
	explicit
	scoped_camera(
		insula::graphics::camera &_camera,
		vec3 const &new_position,
		gizmo const &new_gizmo);

	~scoped_camera();
private:
	insula::graphics::camera &camera_;
	insula::graphics::vec3 const position_;
	insula::graphics::gizmo const gizmo_;
};
}
}

#endif
