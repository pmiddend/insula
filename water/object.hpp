#ifndef INSULA_WATER_OBJECT_HPP_INCLUDED
#define INSULA_WATER_OBJECT_HPP_INCLUDED

#include "../graphics/camera_fwd.hpp"
#include "../graphics/scalar.hpp"
#include "../graphics/shader.hpp"
#include "../graphics/shader_to_console.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/target_ptr.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/dim_type.hpp>
#include <sge/image/multi_loader_fwd.hpp>
#include <sge/console/object_fwd.hpp>
#include <functional>

namespace insula
{
namespace water
{
class object
{
public:
	object(object const &) = delete;
	object &operator=(object const &) = delete;

	explicit 
	object(
		sge::renderer::device_ptr,
		graphics::camera &,
		graphics::scalar water_height,
		sge::image::multi_loader &,
		sge::console::object &,
		graphics::scalar dimension,
		sge::renderer::dim_type const &,
		sge::image::file_ptr );

	void
	update_reflection(
		std::function<void ()> const &render_callback);

	void 
	render();
private:
	sge::renderer::device_ptr const renderer_;
	sge::renderer::texture_ptr target_texture_,bump_texture_;
	sge::renderer::target_ptr target_;
	graphics::camera &camera_;
	graphics::scalar const water_height_;
	sge::image::multi_loader &image_loader_;
	graphics::shader shader_;
	graphics::shader_to_console shader_console_;
	sge::renderer::vertex_buffer_ptr vb_;

	void
	regenerate(
		graphics::scalar,
		sge::renderer::dim_type const &);
};
}
}

#endif
