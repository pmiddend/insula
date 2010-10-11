#ifndef INSULA_ARROW_HPP_INCLUDED
#define INSULA_ARROW_HPP_INCLUDED

#include "arrow_parameters_fwd.hpp"
#include "model/object.hpp"
#include "graphics/shader/object_fwd.hpp"
#include "graphics/camera/object_fwd.hpp"
#include "graphics/vec3.hpp"
#include "physics/vec3.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <functional>

namespace insula
{
class arrow
{
public:
	arrow(arrow const &) = delete;
	arrow &operator=(arrow const &) = delete;

	typedef
	std::function<physics::vec3 ()>
	position_function;

	explicit
	arrow(
		arrow_parameters const &);

	void
	render();

	~arrow();
private:
	model::object model_;
	sge::renderer::texture_ptr texture_;
	graphics::shader::object &model_shader_;
	sge::renderer::device_ptr renderer_;
	graphics::camera::object &camera_;
	position_function position_callback_;
	graphics::vec3 const offset_;
};
}

#endif
