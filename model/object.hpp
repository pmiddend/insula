#ifndef INSULA_MODEL_OBJECT_HPP_INCLUDED
#define INSULA_MODEL_OBJECT_HPP_INCLUDED

#include "../graphics/camera/object_fwd.hpp"
#include "../graphics/shader_fwd.hpp"
#include "../graphics/box.hpp"
#include "../graphics/mat4.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/index_buffer_ptr.hpp>
#include <sge/model/object_ptr.hpp>
#include <sge/model/scalar.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/optional.hpp>
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
		graphics::camera::object const &,
		sge::model::object_ptr,
		sge::renderer::device_ptr,
		graphics::shader_old &,
		sge::renderer::texture_ptr,
		fcppt::optional<fcppt::string> const &part = 
			fcppt::optional<fcppt::string>());

	void
	render(
		graphics::mat4 const &transformation);

	graphics::box const
	bounding_box() const;

	sge::model::object_ptr const
	raw();

	~object();
private:
	sge::model::object_ptr const raw_;
	graphics::camera::object const &camera_;
	sge::renderer::device_ptr renderer_;
	graphics::shader_old &shader_;
	sge::renderer::texture_ptr const texture_;
	sge::renderer::vertex_buffer_ptr vb_;
	sge::renderer::index_buffer_ptr ib_;
	graphics::box bounding_box_;
};
}
}

#endif
