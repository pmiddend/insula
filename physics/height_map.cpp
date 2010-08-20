#include "height_map.hpp"
#include "to_bullet_type.hpp"
#include "transform_from_vec3.hpp"
#include "scalar.hpp"
#include "world.hpp"
#include "../height_map/scalar.hpp"
#include <LinearMath/btDefaultMotionState.h>
#include <LinearMath/btVector3.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/io/cout.hpp>
#include <algorithm>
#include <type_traits>

insula::physics::height_map::height_map(
	world &_world,
	insula::height_map::array const &_array,
	insula::height_map::scalar const grid_size,
	insula::height_map::scalar const height_scaling)
:
	world_(
		_world),
	array_(
		_array)
{
	static_assert(
		std::is_same<insula::height_map::scalar,scalar>::value,
		"Currently, btScalar has to be the same as height_map::scalar. This can be easily fixed.");

	std::pair
	<
		insula::height_map::scalar *,
		insula::height_map::scalar *
	> const minmax = 
		std::minmax_element(
			array_.data(),
			array_.data() + array_.num_elements());

	shape_.reset(
		new btHeightfieldTerrainShape(
			static_cast<int>(
				array_.shape()[0]),
			static_cast<int>(
				array_.shape()[1]),
			array_.data(),
			// heightScale is only used when the type is not float
			static_cast<scalar>(
				1),
			*(minmax.first),
			*(minmax.second),
			// The up axis index (type is int)
			1,
			to_bullet_type<scalar>::value,
			// This determines how the quads are triangulated (there are two
			// possibilities)
			true));


	// Set local scaling (grid sizes)
	shape_->setLocalScaling(
		btVector3(
			grid_size,
			height_scaling,
			grid_size));

	// The position of the height field is the center of the AABB
	motion_state_.reset(
		new btDefaultMotionState(
			transform_from_vec3(
				vec3(
					// Why the - 1/2*grid size here? It's a bug in btHeightfieldTerrainShape (or an oddity at least)
					static_cast<scalar>(array_.shape()[0]/2) * grid_size - static_cast<scalar>(0.5)*grid_size,
					(*(minmax.first) + *(minmax.second)) / 2 * height_scaling,
					static_cast<scalar>(array_.shape()[0]/2) * grid_size- static_cast<scalar>(0.5)*grid_size))));

	body_.reset(
		new btRigidBody(
			btRigidBody::btRigidBodyConstructionInfo(
				// The mass - has to be _exactly_ zero.
				static_cast<scalar>(0.0),
				motion_state_.get(),
				shape_.get(),
				// The local inertia. The mass is zero, so this is zero.
				btVector3(
					0,
					0,
					0))));

	world_.add(
		*body_);
}

insula::physics::height_map::~height_map()
{
	world_.remove(
		*body_);
}
