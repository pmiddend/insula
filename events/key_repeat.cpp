#include "key_repeat.hpp"

insula::events::key_repeat::key_repeat(
	sge::input::key_type const &_k)
:
	k_(
		k_)
{
}

sge::input::key_type const &
insula::events::key_repeat::pair() const
{
	return k_;
}
