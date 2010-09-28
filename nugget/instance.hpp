#ifndef INSULA_NUGGET_INSTANCE_HPP_INCLUDED
#define INSULA_NUGGET_INSTANCE_HPP_INCLUDED

#include "manager_fwd.hpp"
#include "../physics/world_fwd.hpp"
#include "../static_model_instance.hpp"
#include "../physics/vec3.hpp"
#include "empty_callback.hpp"
#include <fcppt/signal/object.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
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

	void
	update();

	// We need this to calculate the arrow's orientation
	physics::vec3 const
	closest_nugget(
		physics::vec3 const &) const;
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
};
}
}

#endif
