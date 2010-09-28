#include "transform_to_gizmo.hpp"
#include "bullet_to_mat3.hpp"
#include "bullet_to_vec3.hpp"
#include "../gizmo/from_mat3.hpp"

insula::physics::gizmo const
insula::physics::transform_to_gizmo(
	btTransform const &t)
{
	gizmo result = 
		insula::gizmo::from_mat3<physics::scalar>(
			bullet_to_mat3(
				t.getBasis()));
	result.position(
		bullet_to_vec3(
			t.getOrigin()));
	return result;
}
