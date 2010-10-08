#ifndef INSULA_SHADOW_OBJECT_HPP_INCLUDED
#define INSULA_SHADOW_OBJECT_HPP_INCLUDED

#include "parameters_fwd.hpp"
#include "../graphics/gizmo.hpp"
#include "../graphics/mat4.hpp"
#include <sge/renderer/texture_ptr.hpp>
#include <sge/renderer/target_ptr.hpp>

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

	graphics::mat4 const
	mvp(
		graphics::mat4 const &projection) const;

	// This is just to test the camera in freelook
	graphics::gizmo const &gizmo() const { return sun_gizmo_; }
private:
	sge::renderer::texture_ptr target_texture_;
	sge::renderer::target_ptr target_;
	graphics::gizmo sun_gizmo_;
};
}
}

#endif
