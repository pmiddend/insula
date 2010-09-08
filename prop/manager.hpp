#ifndef INSULA_PROP_MANAGER_HPP_INCLUDED
#define INSULA_PROP_MANAGER_HPP_INCLUDED

#include "blueprint.hpp"
#include "parameters_fwd.hpp"
#include "../model_backend.hpp"
#include "../physics/shared_shape_ptr.hpp"
#include <boost/ptr_container/ptr_vector.hpp>
#include <sge/parse/json/object.hpp>

namespace insula
{
namespace prop
{
class manager
{
public:
	explicit
	manager(
		parameters const &);

	//instance_sequence const
	//instantiate();

	~manager();
private:
	typedef
	boost::ptr_vector<model_backend>
	backend_sequence;

	typedef
	boost::ptr_vector<blueprint>
	blueprint_sequence;

	backend_sequence backends_;
	blueprint_sequence blueprints_;

	void
	parse_single_prop(
		parameters const &,
		sge::parse::json::object const &);

	physics::shared_shape_ptr const
	parse_shape(
		sge::parse::json::object const &);
};
}
}

#endif
