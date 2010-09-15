#ifndef INSULA_PHYSICS_BROADPHASE_MANAGER_HPP_INCLUDED
#define INSULA_PHYSICS_BROADPHASE_MANAGER_HPP_INCLUDED

#include "../../graphics/camera/object_fwd.hpp"
#include "iteration.hpp"
#include "shared_object_ptr.hpp"
#include <boost/intrusive/list.hpp>

namespace insula
{
namespace physics
{
namespace broadphase
{
class manager
{
public:
	manager(manager const &) = delete;
	manager &operator=(manager const &) = delete;

	explicit
	manager(
		graphics::camera::object const &);

	shared_object_ptr const
	create();

	void
	update();

	iteration
	current_iteration() const;
private:
	typedef
	boost::intrusive::list
	<
		intrusive_wrapper,
		boost::intrusive::constant_time_size<false>
	>
	object_list;

	graphics::camera::object const &camera_;
	object_list objects_;
	iteration current_iteration_;
};
}
}
}

#endif
