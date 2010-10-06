#include "manager.hpp"
#include "render_pass_impl.hpp"
#include "backend.hpp"
#include "transparency_comparator.hpp"
#include "scoped_backend.hpp"
#include "../graphics/camera/object.hpp"
#include "../timed_output.hpp"
#include "../stdlib/keys_to_sequence.hpp"
#include "../stdlib/sort.hpp"
#include <boost/foreach.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/assert.hpp>
#include <set>

insula::scene::manager::manager(
	graphics::camera::object &_camera)
:
	camera_(_camera)
{
}

void
insula::scene::manager::insert(
	backend &b,
	instance &inst)
{
	// This uses the default-ctor of the intrusive list to construct it
	backend_instance_map_[&b].push_back(
		inst);
}

void
insula::scene::manager::insert_transparent(
	backend &b,
	transparent_instance &inst)
{
	inst.backend(
		&b);
	transparent_instances_.push_back(
		inst);
}

void
insula::scene::manager::render()
{
	typedef 
	std::vector<backend_ptr> 
	backend_sequence;

	// We want the backends sorted by priority, but we cannot use the
	// map comparator because that would make backends with the same
	// priority equal and thus "discard" new backends on insertion
	BOOST_FOREACH(
		backend_ptr b,
		stdlib::sort(
			stdlib::keys_to_sequence<backend_sequence>(
				backend_instance_map_),
			[](
				backend_ptr const a,
				backend_ptr const b) 
			{
				return a->priority() < b->priority();
			}))
	{
		instance_list &instances = 
			backend_instance_map_[b];

		// This cannot be here anymore since we might have backends with
		// no instances (like the skydome)
		//if (instances.empty())
		//	continue;

		scoped_backend scoped_backend_(
			b);

		BOOST_FOREACH(
			instance_list::reference instance_ref,
			instances)
			if (instance_ref.is_visible())
				instance_ref.render(
					*b);
	}

	render_transparent();
}

insula::scene::scoped_render_pass const
insula::scene::manager::register_pass(
	render_pass::type const type,
	render_pass_callback const &rp)
{
	return 
		scoped_render_pass(
			new render_pass_impl(
				*this,
				type,
				rp));
}

insula::scene::manager::~manager() {}

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
		if (r.is_visible())
			ordered_instances.insert(
				&r);

	//timed_output() << "There are " << ordered_instances.size() << " transparent props\n";

	BOOST_FOREACH(
		set_type::value_type const &v,
		ordered_instances)
	{
		scoped_backend scoped_backend_(
			v->backend());
		v->render(
			*(v->backend()));
	}
}

void
insula::scene::manager::add(
	backend &b)
{
	backend_ptr bptr = &b;
	backend_instance_map_.insert(
		bptr,
		new instance_list());
}

void
insula::scene::manager::remove(
	backend &b)
{
	backend_instance_map_.erase(
		&b);
}

void 
insula::scene::manager::add(
	render_pass::type const type,
	render_pass_callback const &rp)
{
	FCPPT_ASSERT(
		render_pass_map_.find(type) == render_pass_map_.end());
	render_pass_map_.insert(
		render_pass_map::value_type(
			type,
			rp));
}

void 
insula::scene::manager::remove(
	render_pass::type const rp)
{
	render_pass_map_.erase(
		rp);
}
