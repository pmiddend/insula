#ifndef INSULA_EVENTS_MOUSE_BUTTON_HPP_INCLUDED
#define INSULA_EVENTS_MOUSE_BUTTON_HPP_INCLUDED

#include <sge/input/mouse/button_event.hpp>
#include <boost/statechart/event.hpp>

namespace insula
{
namespace events
{
class mouse_button
:
	public boost::statechart::event<mouse_button>
{
public:
	explicit
	mouse_button(
		sge::input::mouse::button_event const &);

	sge::input::mouse::button_event const &
	event() const;
private:
	sge::input::mouse::button_event k_;
};
}
}

#endif
