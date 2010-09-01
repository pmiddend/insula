#ifndef INSULA_MODEL_OBJECT_HPP_INCLUDED
#define INSULA_MODEL_OBJECT_HPP_INCLUDED

#include "../graphics/box.hpp"
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
/**
	An object takes an sge::model and transforms it into a vertex- and
	an index buffer. 

	It doesn't have a shader because you do not always have _one_ shader
	for a model. To draw shadows, for example, you have two shaders: the
	main shader and the stripped-down shadow shader. Because a shader
	and a texture are somewhat intertwined, the model also doesn't have
	a texture.

	It doesn't have a position because often you want to draw the same
	model 'n' times. So the render function gets a transformation
	instead.
 */
public:
	object(object const &) = delete;
	object &operator=(object const &) = delete;

	explicit 
	object(
		sge::model::object_ptr,
		sge::renderer::device_ptr,
		fcppt::optional<fcppt::string> const &part = 
			fcppt::optional<fcppt::string>());

	void
	render();

	graphics::box const
	bounding_box() const;

	// scoped needs this
	sge::renderer::vertex_buffer_ptr const
	vb();

	~object();
private:
	sge::renderer::device_ptr renderer_;
	sge::renderer::vertex_buffer_ptr vb_;
	sge::renderer::index_buffer_ptr ib_;
	graphics::box bounding_box_;
};
}
}

#endif
