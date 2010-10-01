#ifndef INSULA_PHYSICS_OBJECT_TYPE_HPP_INCLUDED
#define INSULA_PHYSICS_OBJECT_TYPE_HPP_INCLUDED

namespace insula
{
namespace physics
{
namespace object_type
{
// Why do we need that:
// - First, it simplifys checking the type of an object. For example,
//   in the overlap filter we just check if object.type() == ghost
//   instead of downcasting the object
// - Second we have more power over the collision callbacks. You can
//   now register a callback just for collisions "vehicle" with "prop"
//   for example - excluding nuggets (which are both static_models
//   from the physics point of view)
enum type
{
	height_map,
	projectile,
	character,
	prop,
	nugget,
	ghost
};
}
}
}

#endif
