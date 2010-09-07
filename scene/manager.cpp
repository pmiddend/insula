#include "manager.hpp"
#include "transparency_comparator.hpp"
#include "scoped_backend.hpp"
#include "../graphics/camera/object.hpp"
#include <boost/foreach.hpp>
#include <set>

insula::scene::manager::manager(
	graphics::camera::object &_camera)
:
	camera_(_camera)
{
}

void
insula::scene::manager::insert(
	backend_ptr const b,
	instance &inst)
{
	// This uses the default-ctor of the intrusive list to construct it
	backend_instance_map_[b].push_back(
		inst);
}

void
insula::scene::manager::insert_transparent(
	backend_ptr const b,
	transparent_instance &inst)
{
	inst.backend(
		b);
	transparent_instances_.push_back(
		inst);
}

void
insula::scene::manager::render()
{
	// Iteration of a ptr_map is done with value_type (const_reference
	// is something odd). The value_type _has to be const_!
	// - first will be key_type, 
	// - second will be value_type* (not a reference, sadly)
	BOOST_FOREACH(
		backend_instance_map::value_type const &r,
		backend_instance_map_)
	{
		scoped_backend scoped_backend_(
			r.first);

		BOOST_FOREACH(
			instance_list::reference instance_ref,
			*r.second)
			instance_ref.render();
	}
}

void
insula::scene::manager::render_transparent()
{
	// We can't use a ptr_set here because the value_type is not const
	// (we need to call render on the ordered primitives, which is
	// non-const)
	typedef
	std::set
	<
		transparent_instance*,
		transparency_comparator
	>
	set_type;

	set_type ordered_instances(
		transparency_comparator(
			camera_.gizmo().position()));

	BOOST_FOREACH(
		transparent_instance_list::reference r,
		transparent_instances_)
		ordered_instances.insert(
			&r);

	BOOST_FOREACH(
		set_type::value_type const &v,
		ordered_instances)
	{
		scoped_backend scoped_backend_(
			v->backend());
		v->render();
	}
}
