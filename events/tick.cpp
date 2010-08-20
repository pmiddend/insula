#include "tick.hpp"

insula::events::tick::tick(
	time_delta const _delta)
:
	delta_(
		_delta)
{
}

insula::time_delta
insula::events::tick::delta() const
{
	return delta_;
}
