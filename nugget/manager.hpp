#ifndef INSULA_NUGGET_MANAGER_HPP_INCLUDED
#define INSULA_NUGGET_MANAGER_HPP_INCLUDED

#include "parameters_fwd.hpp"
#include "../sound_controller.hpp"
#include "../model/backend.hpp"
#include "../physics/vec3.hpp"
#include "../physics/world_fwd.hpp"
#include "../physics/shared_shape_ptr.hpp"
#include "../physics/scalar.hpp"
#include "../scene/manager_fwd.hpp"
#include "../height_map/flatness_range.hpp"
#include "../height_map/object_fwd.hpp"
#include "../height_map/scalar.hpp"
#include "../physics/object_fwd.hpp"
#include "../physics/world_fwd.hpp"
#include "../physics/broadphase/manager_fwd.hpp"
#include "../physics/rigid/object_fwd.hpp"
#include "../random_engine.hpp"
#include "../rigid_model.hpp"
#include <fcppt/signal/scoped_connection.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <vector>
#include <set>

namespace insula
{
namespace nugget
{
/**
	The nugget manager has ownership over all nuggets. It registers when
	a nugget is collected and spawns a new one in that case.
	*/
class manager
{
public:
	explicit
	manager(
		parameters const &);

	void
	update();

	physics::vec3 const
	closest_nugget(
		physics::vec3 const &) const;
private:
	typedef
	boost::ptr_vector<rigid_model>
	model_sequence;

	typedef
	std::set<physics::rigid::object*>
	deletion_set;

	physics::world &world_;
	height_map::object &height_map_;
	model::backend backend_;
	model_sequence models_;
	physics::shared_shape_ptr shape_;
	scene::manager &scene_manager_;
	physics::broadphase::manager &broadphase_manager_;
	sound_controller &sounds_;
	random_engine rng_;
	physics::scalar floating_height_;
	height_map::scalar water_level_;
	height_map::flatness_range flatness_range_;
	deletion_set to_delete_;
	fcppt::signal::scoped_connection callback_connection_;

	void
	spawn();

	void
	contact_callback(
		physics::rigid::object &,
		physics::rigid::object &);
};
}
}

#endif
