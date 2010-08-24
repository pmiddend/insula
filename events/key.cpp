#include "key.hpp"

insula::events::key::key(
	sge::input::key_pair const &_k)
:
	k_(
		_k)
{
}

sge::input::key_pair const &
insula::events::key::pair() const
{
	return k_;
}
