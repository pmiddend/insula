#ifndef INSULA_MODEL_OBJECT_HPP_INCLUDED
#define INSULA_MODEL_OBJECT_HPP_INCLUDED

#include "../graphics/camera_fwd.hpp"
#include "../graphics/shader_fwd.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/index_buffer_ptr.hpp>
#include <sge/model/object_ptr.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <fcppt/filesystem/path.hpp>

namespace insula
{
namespace model
{
class object
{
public:
	object(object const &) = delete;
	object &operator=(object const &) = delete;

	explicit 
	object(
		graphics::camera const &,
		sge::model::object_ptr,
		sge::renderer::device_ptr,
		graphics::shader &,
		sge::renderer::texture_ptr);

	void
	render();
private:
	graphics::camera const &camera_;
	sge::renderer::device_ptr renderer_;
	graphics::shader &shader_;
	sge::renderer::texture_ptr const texture_;
	sge::renderer::vertex_buffer_ptr vb_;
	sge::renderer::index_buffer_ptr ib_;
};
}
}

#endif
