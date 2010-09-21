#include "instance.hpp"
#include "manager.hpp"
#include "../scene/manager.hpp"
#include "../graphics/mat4.hpp"
#include "../static_model_instance.hpp"
#include "../physics/static_model_parameters.hpp"
#include "../physics/static_model.hpp"
#include "../physics/vehicle/object.hpp"
#include "../physics/object_type.hpp"
#include "../physics/mat3.hpp"
#include "../physics/solidity.hpp"
#include "../physics/world.hpp"
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/matrix/basic_impl.hpp>
#include <fcppt/math/vector/structure_cast.hpp>
#include <fcppt/math/vector/length.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <fcppt/algorithm/ptr_container_erase.hpp>
#include <fcppt/assert.hpp>
#include <boost/foreach.hpp>
#include <algorithm>
#include <functional>
#include <limits>

insula::nugget::instance::instance(
	manager &_manager,
	physics::world &_world)
:
	manager_(
		_manager),
	world_(
		_world),
	connection_(
		world_.register_callback
		<
			physics::vehicle::object,
			physics::static_model
		>(
			physics::object_type::vehicle,
			physics::object_type::nugget,
			std::bind(
				&instance::physics_callback,
				this,
				std::placeholders::_1,
				std::placeholders::_2)))
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
					//physics::solidity::nonsolid)));
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

fcppt::signal::auto_connection
insula::nugget::instance::register_empty_callback(
	empty_callback const &e)
{
	return empty_signal_.connect(e);
}

void
insula::nugget::instance::update()
{
	BOOST_FOREACH(
		static_model_instance const *m,
		to_delete_)
	{
		fcppt::algorithm::ptr_container_erase(
			models_,
			m);
		manager_.sounds_.play(
			FCPPT_TEXT("score"));
	}
	to_delete_.clear();
}

void
insula::nugget::instance::physics_callback(
	physics::vehicle::object &,
	physics::static_model &s)
{
	model_sequence::const_iterator i = 
		std::find_if(
			models_.begin(),
			models_.end(),
			[&s](static_model_instance const &m) 
			{
				return &m.physics_model() == &s;
			});

	// It's not a nugget model
	if (i == models_.end())
		return;

	to_delete_.insert(&(*i));

	if (models_.size() == 1)
		empty_signal_();
}

insula::physics::vec3 const
insula::nugget::instance::closest_nugget(
	physics::vec3 const &ref) const
{
	FCPPT_ASSERT(!models_.empty());

	physics::scalar min_dist = 
		std::numeric_limits<physics::scalar>::max();
	physics::vec3 min;

	BOOST_FOREACH(
		static_model_instance const &sm,
		models_)
	{
		physics::scalar const new_dist = 
			fcppt::math::vector::length(
				ref - sm.physics_model().position()); 

		if (new_dist < min_dist)
		{
			min_dist = new_dist;
			min = sm.physics_model().position();
		}
	}

	return min;
}
