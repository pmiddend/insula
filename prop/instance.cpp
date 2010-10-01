#include "instance.hpp"
#include "instance_parameters.hpp"
#include "manager.hpp"
#include "../scene/manager.hpp"
#include "../physics/object_type.hpp"
#include "../physics/scalar.hpp"
#include "../physics/rigid/parameters.hpp"
#include "../graphics/vec3.hpp"
#include "../graphics/vec4.hpp"
#include "../graphics/mat4.hpp"
#include "../rigid_model.hpp"
#include "../math/uniform_scaling_matrix.hpp"
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/matrix/rotation_axis.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/vector.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/construct.hpp>
#include <fcppt/math/vector/narrow_cast.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <boost/foreach.hpp>

namespace
{
template<typename T>
typename
fcppt::math::matrix::static_<T,3,3>::type const
matrix_to_3x3(
	typename fcppt::math::matrix::static_<T,4,4>::type const &a)
{
	return 
		typename fcppt::math::matrix::static_<T,3,3>::type(
			a[0][0],a[0][1],a[0][2],
			a[1][0],a[1][1],a[1][2],
			a[2][0],a[2][1],a[2][2]);
}
}

insula::prop::instance::instance(
	instance_parameters const &params)
{
	BOOST_FOREACH(
		manager::blueprint_sequence::const_reference r,
		params.manager_.blueprints_)
	{
		graphics::mat4 const 
			model_matrix = 
				fcppt::math::matrix::translation(
					fcppt::math::vector::structure_cast<graphics::vec3>(
						r.origin)) *
				fcppt::math::matrix::rotation_axis(
					r.rotation_angle,
					r.rotation_axis) *
				math::uniform_scaling_matrix(
					r.scaling),
			physics_to_model = 
				fcppt::math::matrix::translation(
					fcppt::math::vector::structure_cast<graphics::vec3>(
						-r.offset))
				 * math::uniform_scaling_matrix(
						r.scaling);

		instances_.push_back(
			new rigid_model(
				physics_to_model,
				params.manager_.broadphase_manager_,
				physics::rigid::parameters(
					params.world,
					physics::object_type::prop,
					// Let's find out what the origin of the "child" shape is in
					// relation to the model. We translate the offset with the same
					// translation matrices as the model and add a translation to the
					// model origin
					r.origin + 
					fcppt::math::vector::narrow_cast<physics::vec3>(
						model_matrix * 
						fcppt::math::vector::structure_cast<graphics::vec4>(
							fcppt::math::vector::construct(
								r.offset,
								static_cast<physics::scalar>(
									0)))),
					// We cannot scale a static model and we cannot translate it
					// with the matrix, so all that's left is the rotation
					matrix_to_3x3<physics::scalar>(
						fcppt::math::matrix::rotation_axis(
							r.rotation_angle,
							r.rotation_axis)),
					r.shape,
					physics::rigid::solidity::solid)));

		if (r.backend.has_transparency())
			params.manager_.scene_manager_.insert_transparent(
				r.backend,
				instances_.back());
		else
			params.manager_.scene_manager_.insert(
				r.backend,
				instances_.back());
	}
}
