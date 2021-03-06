#include "object.hpp"

insula::physics::object::object(
	object_type::type const _type)
:
	last_seen_(
		0),
	type_(
		_type)
{
}

void
insula::physics::object::last_seen(
	broadphase::iteration const &_last_seen)
{
	last_seen_ = _last_seen;
}

insula::physics::broadphase::iteration
insula::physics::object::last_seen() const
{
	return last_seen_;
}

insula::physics::object_type::type
insula::physics::object::type() const
{
	return type_;
}

insula::physics::object::~object()
{
}
