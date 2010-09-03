#include "object.hpp"

insula::physics::object::object()
:
	last_seen_(
		static_cast<iteration>(0))
{
}

void
insula::physics::object::last_seen(
	iteration const &_last_seen)
{
	last_seen_ = _last_seen;
}

insula::physics::iteration
insula::physics::object::last_seen() const
{
	return last_seen_;
}

insula::physics::object::~object()
{
}
