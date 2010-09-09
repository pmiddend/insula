#ifndef INSULA_PROP_MANAGER_HPP_INCLUDED
#define INSULA_PROP_MANAGER_HPP_INCLUDED

#include "blueprint.hpp"
#include "instance_sequence.hpp"
#include "parameters_fwd.hpp"
#include "../scene/manager_fwd.hpp"
#include "../model_backend.hpp"
#include "../physics/shared_shape_ptr.hpp"
#include "../physics/approximation/variant.hpp"
#include "../physics/scalar.hpp"
#include "../model/object_fwd.hpp"
#include <boost/ptr_container/ptr_vector.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/model/object_ptr.hpp>

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

	void
	instantiate(
		instance_sequence &,
		physics::world &);

	~manager();
private:
	typedef
	boost::ptr_vector<model_backend>
	backend_sequence;

	typedef
	boost::ptr_vector<blueprint>
	blueprint_sequence;

	scene::manager &scene_manager_;
	backend_sequence backends_;
	blueprint_sequence blueprints_;

	void
	parse_single_prop(
		parameters const &,
		sge::parse::json::object const &);

	physics::approximation::variant const
	parse_approximation(
		sge::parse::json::object const &,
		physics::scalar const scaling,
		model::object const &);
};
}
}

#endif
