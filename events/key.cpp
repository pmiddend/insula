#include "key.hpp"

insula::events::key::key(
	sge::input::keyboard::key_event const &_k)
:
	k_(
		_k)
{
}

sge::input::keyboard::key_event const &
insula::events::key::event() const
{
	return k_;
}
