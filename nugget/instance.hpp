#ifndef INSULA_NUGGET_INSTANCE_HPP_INCLUDED
#define INSULA_NUGGET_INSTANCE_HPP_INCLUDED

#include "manager_fwd.hpp"
#include "../physics/world_fwd.hpp"
#include "../static_model_instance.hpp"
#include <boost/ptr_container/ptr_vector.hpp>

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
private:
	typedef
	boost::ptr_vector<static_model_instance>
	model_sequence;

	manager &manager_;
	physics::world &world_;
	model_sequence models_;
};
}
}

#endif
