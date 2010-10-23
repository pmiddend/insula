#ifndef INSULA_SHADOW_OBJECT_HPP_INCLUDED
#define INSULA_SHADOW_OBJECT_HPP_INCLUDED

#include "parameters_fwd.hpp"
#include "update_callback_fn.hpp"
#include "update_callback.hpp"
#include "../graphics/gizmo.hpp"
#include "../graphics/vec3.hpp"
#include "../graphics/mat4.hpp"
#include "../graphics/scalar.hpp"
#include "../time_delta.hpp"
#include <sge/renderer/texture_ptr.hpp>
#include <sge/renderer/target_ptr.hpp>
#include <fcppt/signal/object.hpp>
#include <fcppt/signal/auto_connection.hpp>

namespace insula
{
namespace shadow
{
class object
{
public:
	explicit
	object(
		parameters const &);

	// The height map needs the getters
	sge::renderer::texture_ptr const
	texture();

	void
	update(
		time_delta);

	/*
	// This gets a perspective matrix so that shadow::object doesn't
	// need to know the camera
	graphics::mat4 const
	mvp(
		graphics::mat4 const &projection) const;
	*/

	// This is just to test the camera in freelook
	graphics::gizmo const 
	gizmo() const;

	// We need a setter for this because the shadow object is
	// initialized after the heightmap
	void
	base_position(
		graphics::vec3 const &);

	// Here, systems can register for a callback that's called when the
	// sun position (and thus the shadow texture) is updated
	fcppt::signal::auto_connection
	register_callback(
		update_callback const &);
private:
	fcppt::signal::object<update_callback_fn> update_signal_;
	sge::renderer::texture_ptr target_texture_;
	sge::renderer::target_ptr target_;
	// The distance is the distance from the origin. The sun will move
	// on a hemi-sphere and the angle will be updated
	graphics::scalar const sun_distance_;
	graphics::scalar sun_angle_;
	graphics::vec3 base_position_;
};
}
}

#endif
