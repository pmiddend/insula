#ifndef INSULA_PHYSICS_MOTION_STATE_HPP_INCLUDED
#define INSULA_PHYSICS_MOTION_STATE_HPP_INCLUDED

#include "vec3.hpp"
#include "mat3.hpp"
#include <fcppt/signal/object.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <LinearMath/btMotionState.h>
#include <LinearMath/btTransform.h>
#include <functional>

namespace insula
{
namespace physics
{
class motion_state
:
	public btMotionState
{
public:
	motion_state(motion_state const &) = delete;
	motion_state &operator=(motion_state const &) = delete;

	typedef
	void 
	callback_fn(
		btTransform const &);

	typedef
	std::function<callback_fn>
	callback;

	explicit
	motion_state(
		vec3 const &,
		mat3 const &);

	fcppt::signal::auto_connection
	register_callback(
		callback const &);

	btTransform const &
	transform() const;
private:
	typedef
	fcppt::signal::object<callback_fn>
	signal;

	signal signal_;
	btTransform transform_;

	// @override
	void
	getWorldTransform(
		btTransform &) const;

	// @override
	void
	setWorldTransform(
		btTransform const &);
};
}
}

#endif
