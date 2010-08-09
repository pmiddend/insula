#ifndef INSULA_MODEL_OBJECT_HPP_INCLUDED
#define INSULA_MODEL_OBJECT_HPP_INCLUDED

#include "../graphics/camera_fwd.hpp"
#include "../graphics/shader_fwd.hpp"
#include "../graphics/box.hpp"
#include "../graphics/mat4.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/index_buffer_ptr.hpp>
#include <sge/model/object_ptr.hpp>
#include <sge/model/scalar.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <fcppt/string.hpp>

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
		fcppt::string const &part,
		graphics::camera const &,
		sge::model::object_ptr,
		sge::renderer::device_ptr,
		graphics::shader &,
		sge::renderer::texture_ptr,
		sge::model::scalar const scaling = 
			static_cast<sge::model::scalar>(1));

	void
	render(
		graphics::mat4 const &transformation);

	graphics::box const
	bounding_box() const;
private:
	graphics::camera const &camera_;
	sge::renderer::device_ptr renderer_;
	graphics::shader &shader_;
	sge::renderer::texture_ptr const texture_;
	sge::renderer::vertex_buffer_ptr vb_;
	sge::renderer::index_buffer_ptr ib_;
	graphics::box bounding_box_;
};
}
}

#endif
