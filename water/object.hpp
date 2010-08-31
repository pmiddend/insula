#ifndef INSULA_WATER_OBJECT_HPP_INCLUDED
#define INSULA_WATER_OBJECT_HPP_INCLUDED

#include "../graphics/camera/object_fwd.hpp"
#include "../graphics/scalar.hpp"
#include "../graphics/shader.hpp"
#include "../graphics/rect.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/target_ptr.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <sge/renderer/dim_type.hpp>
#include <sge/time/timer.hpp>
#include <sge/image/file_ptr.hpp>
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
		graphics::camera::object &,
		graphics::scalar water_level,
		graphics::rect const &,
		sge::renderer::dim_type const &reflection_texture_size,
		sge::image::file_ptr bump_texture,
		graphics::scalar texture_scaling,
		graphics::scalar wave_height,
		graphics::scalar wind_speed);

	void
	update_reflection(
		std::function<void ()> const &render_callback);

	void 
	render();

	graphics::shader_old &
	shader();

	graphics::scalar
	water_level() const;

	~object();
private:
	sge::renderer::device_ptr const renderer_;
	sge::renderer::texture_ptr target_texture_,bump_texture_;
	sge::renderer::target_ptr target_;
	graphics::camera::object &camera_;
	graphics::scalar const water_level_;
	graphics::shader_old shader_;
	sge::renderer::vertex_buffer_ptr vb_;
	sge::time::timer wave_timer_;
	graphics::scalar current_time_;

	void
	regenerate(
		graphics::rect const &,
		sge::renderer::dim_type const &,
		graphics::scalar texture_scaling);
};
}
}

#endif
