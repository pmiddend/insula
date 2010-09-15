#ifndef INSULA_PROP_MANAGER_HPP_INCLUDED
#define INSULA_PROP_MANAGER_HPP_INCLUDED

#include "blueprint.hpp"
#include "instance_sequence.hpp"
#include "parameters_fwd.hpp"
#include "shared_instance_ptr.hpp"
#include "../scene/manager_fwd.hpp"
#include "../model/backend.hpp"
#include "../physics/shared_shape_ptr.hpp"
#include "../physics/approximation/variant.hpp"
#include "../physics/triangle_mesh/shape.hpp"
#include "../physics/broadphase/manager_fwd.hpp"
#include "../physics/scalar.hpp"
#include "../physics/world_fwd.hpp"
#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
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

	shared_instance_ptr	const
	instantiate(
		physics::world &);

	~manager();
private:
	friend class instance;

	typedef
	boost::ptr_vector<physics::triangle_mesh::shape>
	mesh_prototype_sequence;

	typedef
	boost::ptr_vector<model::backend>
	backend_sequence;

	typedef
	boost::ptr_vector<blueprint>
	blueprint_sequence;

	scene::manager &scene_manager_;
	physics::broadphase::manager &broadphase_manager_;
	backend_sequence backends_;
	blueprint_sequence blueprints_;
	mesh_prototype_sequence mesh_prototypes_;

	void
	parse_single_prop(
		parameters const &,
		sge::parse::json::object const &);

	physics::approximation::variant const
	parse_approximation(
		sge::parse::json::object const &,
		physics::scalar const scaling);
};
}
}

#endif
