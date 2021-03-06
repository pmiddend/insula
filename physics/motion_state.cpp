#include "motion_state.hpp"
#include "vec3_to_bullet.hpp"
#include "mat3_to_bullet.hpp"

insula::physics::motion_state::motion_state(
	vec3 const &p,
	mat3 const &m)
:
	transform_(
		mat3_to_bullet(
			m),
		vec3_to_bullet(
			p))
{
}

fcppt::signal::auto_connection
insula::physics::motion_state::register_callback(
	callback const &c)
{
	return signal_.connect(c);
}

btTransform const &
insula::physics::motion_state::transform() const
{
	return transform_;
}

void
insula::physics::motion_state::getWorldTransform(
	btTransform &t) const
{
	t = transform_;
}

void
insula::physics::motion_state::setWorldTransform(
	btTransform const &t)
{
	transform_ = t;
	signal_(
		t);
}
