#include "mouse_axis.hpp"

insula::events::mouse_axis::mouse_axis(
	sge::input::mouse::axis_event const &_k)
:
	k_(
		_k)
{
}

sge::input::mouse::axis_event const &
insula::events::mouse_axis::event() const
{
	return k_;
}
