#ifndef INSULA_PHYSICS_height_map_HPP_INCLUDED
#define INSULA_PHYSICS_height_map_HPP_INCLUDED

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
class height_map
{
public:
	height_map(height_map const &) = delete;
	height_map &operator=(height_map const &) = delete;

	explicit
	height_map(
		world &,
		insula::height_map::array const &,
		insula::height_map::scalar grid_size,
		insula::height_map::scalar height_scaling);

	~height_map();
private:
	// This has to be kept because the body doesn't automatically
	// unregister itself from the world
	world &world_;
	// We have to keep this here because the caller is responsible for
	// holding the height field data
	insula::height_map::array array_;
	std::unique_ptr<btHeightfieldTerrainShape> shape_;
	std::unique_ptr<btMotionState> motion_state_;
	std::unique_ptr<btRigidBody> body_;
};
}
}

#endif
