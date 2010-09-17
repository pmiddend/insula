#include "instance.hpp"
#include "manager.hpp"
#include "../scene/manager.hpp"
#include "../graphics/mat4.hpp"
#include "../static_model_instance.hpp"
#include "../physics/static_model_parameters.hpp"
#include "../physics/object_type.hpp"
#include "../physics/mat3.hpp"
#include "../physics/solidity.hpp"
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <boost/foreach.hpp>

insula::nugget::instance::instance(
	manager &_manager,
	physics::world &_world)
:
	manager_(
		_manager),
	world_(
		_world)
{
	BOOST_FOREACH(
		manager::position_sequence::const_reference r,
		manager_.nugget_positions_)
	{
		graphics::mat4 const model_matrix = 
			fcppt::math::matrix::translation(
					fcppt::math::vector::structure_cast<graphics::vec3>(
						r));

		models_.push_back(
			new static_model_instance(
				model_matrix,
				manager_.broadphase_manager_,
				physics::static_model_parameters(
					_world,
					physics::object_type::nugget,
					r,
					physics::mat3::identity(),
					manager_.shape_,
					physics::solidity::nonsolid)));

		if (manager_.backend_.has_transparency())
			manager_.scene_manager_.insert_transparent(
				manager_.backend_,
				models_.back());
		else
			manager_.scene_manager_.insert(
				manager_.backend_,
				models_.back());
	}
}
