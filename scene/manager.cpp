#include "manager.hpp"
#include "backend.hpp"
#include "transparency_comparator.hpp"
#include "scoped_backend.hpp"
#include "render_pass/scoped.hpp"
#include "../exception.hpp"
#include "../graphics/camera/object.hpp"
#include "../timed_output.hpp"
#include "../stdlib/keys_to_sequence.hpp"
#include "../stdlib/sort.hpp"
#include "../stdlib/filter.hpp"
#include <boost/foreach.hpp>
#include <boost/graph/topological_sort.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/assert.hpp>
#include <set>
#include <deque>
#include <iterator>

insula::scene::manager::manager(
	sge::renderer::device_ptr const _renderer,
	graphics::camera::object &_camera)
:
	renderer_(
		_renderer),
	camera_(
		_camera)
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
	boost::graph_traits<render_pass_graph>::vertex_descriptor
	vertex;

	std::deque<vertex> render_deque;

	boost::topological_sort(
		render_pass_graph_,
		std::front_inserter(
			render_deque));

	BOOST_FOREACH(vertex const &v,render_deque)
		render(
			render_passes_[v]);
}

void
insula::scene::manager::add(
	render_pass::object const &rp,
	render_pass::dependency_set const &deps)
{
	render_passes_.push_back(
		rp);
	boost::add_vertex(
		render_pass_graph_);

	BOOST_FOREACH(
		render_pass::identifier const &dep,
		deps)
	{
		render_pass_sequence::const_iterator depi = 
			std::find_if(
				render_passes_.begin(),
				render_passes_.end(),
				[&dep](render_pass::object const &ref) { return ref.name == dep; });

		if (depi == render_passes_.end())
			throw insula::exception(
				FCPPT_TEXT("Renderpass dependency \"")+
				dep+
				FCPPT_TEXT("\" not found!"));
			
		boost::add_edge(
			std::distance(
				render_pass_sequence::const_iterator(render_passes_.begin()),
				depi),
			render_passes_.size()-1,
			render_pass_graph_);
	}
}

insula::scene::manager::~manager() {}

void
insula::scene::manager::render(
	render_pass::object const &rp)
{
	// Debugging for the shadow maps
	//if (rp.name == FCPPT_TEXT("normal"))
	//	return;
	render_pass::scoped scoped_render(
		renderer_,
		camera_,
		rp);
	
	typedef 
	std::vector<backend_ptr> 
	backend_sequence;

	// We want the backends sorted by priority, but we cannot use the
	// map comparator because that would make backends with the same
	// priority equal and thus "discard" new backends on insertion
	BOOST_FOREACH(
		backend_ptr b,
		stdlib::filter(
			stdlib::sort(
				stdlib::keys_to_sequence<backend_sequence>(
					backend_instance_map_),
				[](
					backend_ptr const a,
					backend_ptr const b) 
				{
					return a->priority() < b->priority();
				}),
			[&rp](backend_ptr const a) 
			{
				return a->takes_part_in(rp.name);
			}))
	{
		instance_list &instances = 
			backend_instance_map_[b];

		// This cannot be here anymore since we might have backends with
		// no instances (like the skydome)
		//if (instances.empty())
		//	continue;

		scoped_backend scoped_backend_(
			b,
			rp);

		BOOST_FOREACH(
			instance_list::reference instance_ref,
			instances)
			if (instance_ref.is_visible())
				instance_ref.render(
					*b);
	}

	render_transparent(
		rp);
}

void
insula::scene::manager::render_transparent(
	scene::render_pass::object const &rp)
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
			v->backend(),
			rp);
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
