#include "mouse_button.hpp"

insula::events::mouse_button::mouse_button(
	sge::input::mouse::button_event const &_k)
:
	k_(
		_k)
{
}

sge::input::mouse::button_event const &
insula::events::mouse_button::event() const
{
	return k_;
}
