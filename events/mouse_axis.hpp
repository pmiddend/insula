#ifndef INSULA_EVENTS_MOUSE_AXIS_HPP_INCLUDED
#define INSULA_EVENTS_MOUSE_AXIS_HPP_INCLUDED

#include <sge/input/mouse/axis_event.hpp>
#include <boost/statechart/event.hpp>

namespace insula
{
namespace events
{
class mouse_axis
:
	public boost::statechart::event<mouse_axis>
{
public:
	explicit
	mouse_axis(
		sge::input::mouse::axis_event const &);

	sge::input::mouse::axis_event const &
	event() const;
private:
	sge::input::mouse::axis_event k_;
};
}
}

#endif
