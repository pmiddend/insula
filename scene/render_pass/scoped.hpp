#ifndef INSULA_SCENE_RENDER_PASS_SCOPED_HPP_INCLUDED
#define INSULA_SCENE_RENDER_PASS_SCOPED_HPP_INCLUDED

#include "object_fwd.hpp"
#include "../../graphics/camera/scoped.hpp"
#include "../../graphics/camera/object_fwd.hpp"
#include <sge/renderer/scoped_target.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/scoped_block.hpp>

namespace insula
{
namespace scene
{
namespace render_pass
{
class scoped
{
public:
	scoped(scoped const &) = delete;
	scoped &operator=(scoped const &) = delete;

	explicit
	scoped(
		sge::renderer::device_ptr,
		graphics::camera::object &,
		object const &);
private:
	graphics::camera::scoped camera_;
	sge::renderer::scoped_target target_;
	sge::renderer::scoped_block block_;
};
}
}
}

#endif
