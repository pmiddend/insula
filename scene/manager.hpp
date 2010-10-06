#ifndef INSULA_SCENE_MANAGER_HPP_INCLUDED
#define INSULA_SCENE_MANAGER_HPP_INCLUDED

#include "backend_ptr.hpp"
// For the intrusive_list, we need the complete type
#include "instance.hpp"
#include "render_pass_callback.hpp"
#include "render_pass.hpp"
#include "scoped_render_pass.hpp"
#include "../graphics/camera/object_fwd.hpp"
// For the intrusive_list, we need the complete type
#include "transparent_instance.hpp"
#include <boost/intrusive/list.hpp>
#include <boost/ptr_container/ptr_map.hpp>
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

	scoped_render_pass const
	register_pass(
		render_pass::type,
		render_pass_callback const &);

	~manager();
private:
	friend class backend;
	friend class render_pass_impl;

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
	std::map
	<
		render_pass::type,
		render_pass_callback
	>
	render_pass_map;

	typedef
	boost::intrusive::list
	<
		transparent_instance,
		boost::intrusive::constant_time_size<false>
	>
	transparent_instance_list;

	graphics::camera::object &camera_;
	backend_instance_map backend_instance_map_;
	transparent_instance_list transparent_instances_;
	render_pass_map render_pass_map_;

	void
	render_transparent();

	void
	add(
		backend &);

	void
	remove(
		backend &);

	void 
	add(
		render_pass::type,
		render_pass_callback const &);

	void 
	remove(
		render_pass::type);
};
}
}

#endif
