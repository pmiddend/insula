#include "shape_from_approximation.hpp"
#include "dim3_to_bullet.hpp"
#include <fcppt/math/dim/arithmetic.hpp>
#include <fcppt/variant/apply_unary.hpp>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>

namespace
{
class approximation_visitor
{
public:
	typedef 
	insula::physics::shared_shape_ptr
	result_type;

	result_type
	operator()(
		insula::physics::approximation::sphere const &s) const
	{
		return 
			result_type(
				new btSphereShape(
					s.radius));
	}

	result_type
	operator()(
		insula::physics::approximation::box const &b) const
	{
		return 
			result_type(
				new btBoxShape(
					insula::physics::dim3_to_bullet(
						static_cast<insula::physics::scalar>(0.5) * b.size)));
	}
};
}

insula::physics::shared_shape_ptr const
insula::physics::shape_from_approximation(
	approximation::variant const &v)
{
	return 
		fcppt::variant::apply_unary(
			approximation_visitor(),
			v);
}
