#include "raycaster.hpp"
#include "../../timed_output.hpp"
#include "../../height_map/object.hpp"
#include "../object.hpp"
#include "../vec3_to_bullet.hpp"
#include "../../math/barycentric_coordinates.hpp"
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <iostream>

insula::physics::vehicle::raycaster::raycaster(
	btDynamicsWorld &_world,
	height_map::object const &_height_map)
:
	world_(
		_world),
	height_map_(
		_height_map)
{
}

void* 
insula::physics::vehicle::raycaster::castRay(
	btVector3 const & from,
	btVector3 const & to, 
	btVehicleRaycasterResult &result)
{
	btCollisionWorld::ClosestRayResultCallback rayCallback(
		from,
		to);

	world_.rayTest(
		from, 
		to, 
		rayCallback);

	if (!rayCallback.hasHit())
		return 0;

	btRigidBody *body = 
		btRigidBody::upcast(
			rayCallback.m_collisionObject);

	if (!body || !body->hasContactResponse())
		return 0;

	if (static_cast<object *>(body->getUserPointer())->type() == object_type::height_map)
	{
		height_map::vec3 const point(
			rayCallback.m_hitPointWorld.getX(),
			height_map_.project(
				height_map::vec2(
					rayCallback.m_hitPointWorld.getX(),
					rayCallback.m_hitPointWorld.getZ())),
			rayCallback.m_hitPointWorld.getZ());
		/*
		rayCallback.m_hitPointWorld.setY(
			height_map_.project(
				height_map::vec2(
					rayCallback.m_hitPointWorld.getX(),
					rayCallback.m_hitPointWorld.getZ())));*/
		// this shows the difference between bullet's calculated point on
		// the ground and the projected one. Difference was up to 0.05.
		/*
		timed_output() 
			<< (rayCallback.m_hitPointWorld.getY() - height_map_.project(height_map::vec2(rayCallback.m_hitPointWorld.getX(),rayCallback.m_hitPointWorld.getZ()))) 
			<< "\n";*/

		height_map::triangle const t = 
			height_map_.enclosing_triangle(
				height_map::vec2(
					rayCallback.m_hitPointWorld.getX(),
					rayCallback.m_hitPointWorld.getZ()));
		
		height_map::vec3 const barry =
			math::barycentric_coordinates<height_map::scalar>(
				point,
				t.points[0],
				t.points[1],
				t.points[2]);

		result.m_hitNormalInWorld = 
			vec3_to_bullet(
				// FIXME: std::inner_product!
				barry[0] * t.normals[0] + barry[1] * t.normals[1] + barry[2] * t.normals[2]);
	}
	else
		result.m_hitNormalInWorld = rayCallback.m_hitNormalWorld;

	/* Old code
	result.m_hitNormalInWorld = rayCallback.m_hitNormalWorld;
	result.m_hitNormalInWorld.normalize();
	*/


	result.m_hitPointInWorld = rayCallback.m_hitPointWorld;
	result.m_distFraction = rayCallback.m_closestHitFraction;
	return body;
}
