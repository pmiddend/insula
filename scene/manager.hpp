#ifndef INSULA_SCENE_MANAGER_HPP_INCLUDED
#define INSULA_SCENE_MANAGER_HPP_INCLUDED

#include "backend_ptr.hpp"
// For the intrusive_list, we need the complete type
#include "instance.hpp"
#include "../graphics/camera/object_fwd.hpp"
// For the intrusive_list, we need the complete type
#include "transparent_instance.hpp"
#include "render_pass/object.hpp"
#include "render_pass/dependency_set.hpp"
#include <sge/renderer/device_ptr.hpp>
#include <boost/intrusive/list.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <map>

namespace insula
{
namespace scene
{
class manager
{
public:
	manager(manager const &) = delete;
	manager &operator=(manager const &) = delete;

	explicit
	manager(
		sge::renderer::device_ptr,
		graphics::camera::object &);

	void
	insert(
		backend &,
		instance &);

	void
	insert_transparent(
		backend &,
		transparent_instance &);

	void
	render();

	void
	add(
		render_pass::object const &,
		render_pass::dependency_set const &);

	~manager();
private:
	friend class backend;

	typedef
	boost::intrusive::list
	<
		instance,
		boost::intrusive::constant_time_size<false>
	>
	instance_list;

	typedef
	boost::ptr_map
	<
		backend_ptr,
		instance_list
	>
	backend_instance_map;

	typedef
	boost::intrusive::list
	<
		transparent_instance,
		boost::intrusive::constant_time_size<false>
	>
	transparent_instance_list;

	typedef
	std::vector<render_pass::object>
	render_pass_sequence;

	// The graph just indexes into the sequence above
	typedef
	boost::adjacency_list<boost::vecS,boost::vecS,boost::directedS>
	render_pass_graph;

	sge::renderer::device_ptr renderer_;
	graphics::camera::object &camera_;
	backend_instance_map backend_instance_map_;
	transparent_instance_list transparent_instances_;
	render_pass_sequence render_passes_;
	render_pass_graph render_pass_graph_;

	void
	render(
		render_pass::object const &);

	void
	render_transparent(
		render_pass::object const &);

	void
	add(
		backend &);

	void
	remove(
		backend &);
};
}
}

#endif
