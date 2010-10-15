#ifndef INSULA_OVERLAY_OBJECT_HPP_INCLUDED
#define INSULA_OVERLAY_OBJECT_HPP_INCLUDED

#include "function_backend.hpp"
#include "parameters_fwd.hpp"
#include "../graphics/shader/object.hpp"
#include "../graphics/camera/object_fwd.hpp"
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <sge/renderer/texture_ptr.hpp>
#include <sge/renderer/target_ptr.hpp>
#include <sge/renderer/device_ptr.hpp>
#include <sge/renderer/vertex_buffer_ptr.hpp>

namespace insula
{
namespace overlay
{
class object
{
public:
	explicit
	object(
		parameters const &);

	/*
	// The normal render pass (which isn't contained here) has to render
	// into this texture...
	sge::renderer::texture_ptr const
	texture();
	*/

	// using this target
	sge::renderer::target_ptr const
	target();

	// You can register for callbacks which are executed after the
	// texture is drawn
	fcppt::signal::auto_connection
	register_callback(
		function_backend::callback const &);
private:
	sge::renderer::device_ptr renderer_;
	graphics::camera::object &camera_;
	// This backend draws the quad with the overlay texture
	function_backend texture_backend_;
	// And this one is activated after that, to draw...overlay stuff
	function_backend user_backend_;
	sge::renderer::texture_ptr overlay_texture_;
	sge::renderer::target_ptr target_;
	sge::renderer::vertex_buffer_ptr vb_;
	graphics::shader::object quad_shader_;
	fcppt::signal::scoped_connection internal_connection_;

	void
	render();
};
}
}

#endif
