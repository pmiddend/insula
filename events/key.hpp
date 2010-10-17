#ifndef INSULA_EVENTS_KEY_HPP_INCLUDED
#define INSULA_EVENTS_KEY_HPP_INCLUDED

#include <sge/input/keyboard/key_event.hpp>
#include <boost/statechart/event.hpp>

namespace insula
{
namespace events
{
class key
:
	public boost::statechart::event<key>
{
public:
	explicit
	key(
		sge::input::keyboard::key_event const &);

	sge::input::keyboard::key_event const &
	event() const;
private:
	sge::input::keyboard::key_event k_;
};
}
}

#endif
