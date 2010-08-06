#ifndef INSULA_PHYSICS_HEIGHT_FIELD_HPP_INCLUDED
#define INSULA_PHYSICS_HEIGHT_FIELD_HPP_INCLUDED

#include "world_fwd.hpp"
#include "../height_map/array.hpp"
#include "../height_map/scalar.hpp"
#include <memory>

class btHeightfieldTerrainShape;
class btMotionState;
class btRigidBody;

namespace insula
{
namespace physics
{
class height_field
{
public:
	height_field(height_field const &) = delete;
	height_field &operator=(height_field const &) = delete;

	explicit
	height_field(
		world &,
		// NOTE: This array has to be scaled with the height scaling constant!
		height_map::array const &,
		height_map::scalar grid_size);

	~height_field();
private:
	// This has to be kept because the body doesn't automatically
	// unregister itself from the world
	world &world_;
	height_map::array array_;
	std::unique_ptr<btHeightfieldTerrainShape> shape_;
	std::unique_ptr<btMotionState> motion_state_;
	std::unique_ptr<btRigidBody> body_;
};
}
}

#endif
