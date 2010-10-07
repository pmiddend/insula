#ifndef INSULA_WATER_OBJECT_HPP_INCLUDED
#define INSULA_WATER_OBJECT_HPP_INCLUDED

#include "../graphics/camera/object_fwd.hpp"
#include "../graphics/scalar.hpp"
#include "../graphics/shader/object.hpp"
#include "../graphics/rect.hpp"
#include "../scene/backend.hpp"
#include "../scene/manager_fwd.hpp"
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
:
	public scene::backend
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
		graphics::scalar wind_speed,
		scene::manager &);

	void 
	begin(
		scene::render_pass::object const &);

	void
	end(
		scene::render_pass::object const &);

	graphics::scalar
	water_level() const;

	~object();
private:
	sge::renderer::device_ptr const renderer_;
	sge::renderer::texture_ptr target_texture_,bump_texture_;
	sge::renderer::target_ptr target_;
	graphics::camera::object &camera_;
	graphics::scalar const water_level_;
	graphics::shader::object shader_;
	sge::renderer::vertex_buffer_ptr vb_;
	sge::time::timer wave_timer_;
	graphics::scalar current_time_;
};
}
}

#endif
