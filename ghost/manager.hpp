#ifndef INSULA_GHOST_MANAGER_HPP_INCLUDED
#define INSULA_GHOST_MANAGER_HPP_INCLUDED

#include "shared_instance_ptr.hpp"
#include "blueprint.hpp"
#include "manager_parameters_fwd.hpp"
#include "../ghost_instance.hpp"
#include "../model/backend.hpp"
#include "../scene/manager_fwd.hpp"
#include "../physics/broadphase/manager_fwd.hpp"
#include "../physics/broadphase/shared_object_ptr.hpp"
#include <sge/parse/json/array_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

namespace insula
{
namespace ghost
{
/**
	The ghost manager is similar to the prop and nugget manager. It
	reads the ghost items from the json array and creates
	ghost_instances from it as well as the neccesary model backends.

	The big difference is, however, that it stores the instances in a
	separate broadphase. This gives a huge performance boost. Also, we
	don't need and "blueprints" here since the broadphase is never
	recreated (there's no need to). So the ghosts are simpler.
	*/
class manager
{
public:
	manager(manager const &) = delete;
	manager &operator=(manager const &) = delete;

	explicit
	manager(
		manager_parameters const &);

	~manager();
private:
	typedef
	boost::ptr_vector<model::backend>
	backend_sequence;

	typedef
	boost::ptr_vector<ghost_instance>
	instance_sequence;

	scene::manager &scene_manager_;
	physics::broadphase::shared_object_ptr broadphase_;
	backend_sequence backends_;
	instance_sequence instances_;

	void
	parse_single(
		manager_parameters const &,
		sge::parse::json::object const &);
};
}
}

#endif
