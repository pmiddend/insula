#ifndef INSULA_PHYSICS_DEBUG_DRAWER_HPP_INCLUDED
#define INSULA_PHYSICS_DEBUG_DRAWER_HPP_INCLUDED

#include "world_fwd.hpp"
#include "../graphics/shader/object.hpp"
#include "../graphics/camera/object_fwd.hpp"
#include "../graphics/mat4.hpp"
#include "../graphics/vec3.hpp"
#include <sge/renderer/vertex_buffer_ptr.hpp>
#include <LinearMath/btIDebugDraw.h>
#include <tuple>

namespace insula
{
namespace physics
{
/// bullet offers registering a debug drawer to draw contact points or
/// just shapes. This class implements this interface, caching all the
/// lines and drawing them at once (if you don't do that, it'll be
/// super-slow.
class debug_drawer
:
	public btIDebugDraw
{
public:	
	explicit
	debug_drawer(
		world &,
		sge::renderer::device_ptr,
		graphics::camera::object &);

	// @override
	void
	drawLine(
		btVector3 const &from,
		btVector3 const &to,
		btVector3 const &color);

	// @override
	void
	drawLine(
		btVector3 const &from,
		btVector3 const &to,
		btVector3 const &from_color,
		btVector3 const &to_color);

	// @override
	void
	drawContactPoint(
		btVector3 const &PointOnB,
		btVector3 const &normalOnB,
		btScalar distance,
		int lifeTime,
		btVector3 const & color);

	// @override
	void	
	draw3dText(
		btVector3 const&location,
		char const*textString);
	
	// @override
	void
	reportErrorWarning(
		char const* warningString);

	// @override
	void 
	setDebugMode(
		int);

	// @override
	int 
	getDebugMode() const;

	/*
	void
	drawBox(
		btVector3 const& boxMin, 
		btVector3 const& boxMax, 
		btVector3 const& color, 
		btScalar const alpha) {}

	void	
	drawTriangle(
		btVector3 const& v0,
		btVector3 const& v1,
		btVector3 const& v2,
		btVector3 const &color, 
		btScalar alpha) {}
	*/

	void
	render();

	~debug_drawer();
private:
	typedef
	std::vector
	<
		std::tuple
		<
			graphics::vec3,
			graphics::vec3
		>
	>
	line_cache;

	world &world_;
	sge::renderer::device_ptr const renderer_;
	graphics::camera::object &camera_;
	sge::renderer::vertex_buffer_ptr vb_;
	graphics::shader::object shader_;
	int debug_mode_;
	line_cache lines_;
};
}
}

#endif
