#ifndef INSULA_NUGGET_INSTANCE_HPP_INCLUDED
#define INSULA_NUGGET_INSTANCE_HPP_INCLUDED

#include "manager_fwd.hpp"
#include "../physics/world_fwd.hpp"
#include "../physics/vehicle/object_fwd.hpp"
#include "../physics/static_model_fwd.hpp"
#include "../static_model_instance.hpp"
#include "empty_callback.hpp"
#include <fcppt/signal/object.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <set>

namespace insula
{
namespace nugget
{
class instance
{
public:
	explicit
	instance(
		manager &,
		physics::world &);

	fcppt::signal::auto_connection
	register_empty_callback(
		empty_callback const &);

	void
	update();
private:
	typedef
	boost::ptr_vector<static_model_instance>
	model_sequence;

	typedef
	std::set<static_model_instance const *>
	deletion_set;

	manager &manager_;
	physics::world &world_;
	model_sequence models_;
	deletion_set to_delete_;
	fcppt::signal::auto_connection connection_;
	fcppt::signal::object<void ()> empty_signal_;

	void
	physics_callback(
		physics::vehicle::object &,
		physics::static_model &);
};
}
}

#endif
