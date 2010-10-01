#ifndef INSULA_PROJECTILE_MANAGER_HPP_INCLUDED
#define INSULA_PROJECTILE_MANAGER_HPP_INCLUDED

#include "manager_parameters_fwd.hpp"
#include "../physics/vec3.hpp"
#include "../model/backend.hpp"
#include "../rigid_model.hpp"
#include "../scene/manager.hpp"
#include "../physics/broadphase/manager_fwd.hpp"
#include "../physics/world_fwd.hpp"
#include "../physics/scalar.hpp"
#include <boost/ptr_container/ptr_vector.hpp>

namespace insula
{
namespace projectile
{
class manager
{
public:
	explicit
	manager(
		manager_parameters const &);

	void
	spawn(
		physics::vec3 const &position,
		physics::vec3 const &linear_velocity);
private:
	typedef
	boost::ptr_vector<rigid_model>
	model_sequence;

	model::backend backend_;
	model_sequence models_;
	scene::manager &scene_manager_;
	physics::broadphase::manager &broadphase_;
	physics::world &world_;
	physics::scalar mass_;
	physics::shared_shape_ptr shape_;
};
}
}

#endif
