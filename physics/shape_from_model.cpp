#include "shape_from_model.hpp"
#include "scalar.hpp"
#include "dim3_to_bullet.hpp"
#include "vec3.hpp"
#include "../model/object.hpp"
#include <fcppt/math/dim/structure_cast.hpp>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <fcppt/math/dim/structure_cast.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/math/dim/arithmetic.hpp>
#include <fcppt/assert.hpp>

insula::physics::shared_shape_ptr const
insula::physics::shape_from_model(
	model::object &m,
	approximation::numeric_value::type const type)
{
	switch (type)
	{
		case approximation::numeric_value::box:
			return 
				insula::physics::shared_shape_ptr(
					new btBoxShape(
						dim3_to_bullet(
							// btBoxShape gets half extents, so muliply by 0.5 here
							static_cast<scalar>(0.5)*
							fcppt::math::dim::structure_cast<dim3>(
								m.bounding_box().dimension()))));
		case approximation::numeric_value::sphere:
			return 
				insula::physics::shared_shape_ptr(
					new btSphereShape(
						length(
							static_cast<scalar>(0.5) * 
							fcppt::math::dim::structure_cast<vec3>(
								m.bounding_box().dimension()))));
		break;
	}
	FCPPT_ASSERT(false);
}
