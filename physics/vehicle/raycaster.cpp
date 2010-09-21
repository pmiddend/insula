#include "raycaster.hpp"
#include "../../height_map/object.hpp"
#include "../../height_map/calculate_normal.hpp"
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

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

	result.m_hitPointInWorld = rayCallback.m_hitPointWorld;
	/*
	height_map::vf::packed_normal const normal = 
		normalize(
			calculate_normal(
				height_map_.heights(),
				height_map_.height_scaling(),
				height_map_.cell_size(),
				x,
				y));
	*/
	result.m_hitNormalInWorld = rayCallback.m_hitNormalWorld;
	result.m_hitNormalInWorld.normalize();
	result.m_distFraction = rayCallback.m_closestHitFraction;
	return body;
}
