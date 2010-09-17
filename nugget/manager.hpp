#ifndef INSULA_NUGGET_MANAGER_HPP_INCLUDED
#define INSULA_NUGGET_MANAGER_HPP_INCLUDED

#include "parameters_fwd.hpp"
#include "shared_instance_ptr.hpp"
#include "../sound_controller.hpp"
#include "../model/backend.hpp"
#include "../physics/vec3.hpp"
#include "../physics/world_fwd.hpp"
#include "../physics/shared_shape_ptr.hpp"
#include "../scene/manager_fwd.hpp"
#include "../physics/broadphase/manager_fwd.hpp"
#include <fcppt/math/vector/basic_impl.hpp>
#include <vector>

namespace insula
{
namespace nugget
{
class manager
{
public:
	explicit
	manager(
		parameters const &);

	shared_instance_ptr const
	instantiate(
		physics::world &);
private:
	friend class instance;

	// Those are the positions from which the model list is
	// reconstructed each turn. 
	typedef
	std::vector<physics::vec3>
	position_sequence;

	position_sequence nugget_positions_;
	model::backend backend_;
	physics::shared_shape_ptr shape_;
	scene::manager &scene_manager_;
	physics::broadphase::manager &broadphase_manager_;
	sound_controller &sounds_;
};
}
}

#endif
