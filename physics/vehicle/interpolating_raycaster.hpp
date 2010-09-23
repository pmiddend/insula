#ifndef INSULA_PHYSICS_VEHICLE_INTERPOLATING_RAYCASTER_HPP_INCLUDED
#define INSULA_PHYSICS_VEHICLE_INTERPOLATING_RAYCASTER_HPP_INCLUDED

#include "raycaster.hpp"
#include "../../height_map/object_fwd.hpp"

class btDynamicsWorld;

namespace insula
{
namespace physics
{
namespace vehicle
{
class interpolating_raycaster
:
	public raycaster
{
public:
	explicit
	interpolating_raycaster(
		btDynamicsWorld &,
		height_map::object const &);

	raycast_results const
	cast_ray(
		btVector3 const &from,
		btVector3 const &to);
private:
	btDynamicsWorld &world_;
	height_map::object const &height_map_;
};
}
}
}

#endif
