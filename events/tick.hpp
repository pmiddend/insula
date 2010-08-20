#ifndef INSULA_EVENTS_TICK_HPP_INCLUDED
#define INSULA_EVENTS_TICK_HPP_INCLUDED

#include "../time_delta.hpp"
#include <boost/statechart/event.hpp>

namespace insula
{
namespace events
{
class tick
:
	public boost::statechart::event<tick>
{
public:
	explicit
	tick(
		time_delta);

	time_delta
	delta() const;
private:
	time_delta delta_;
};
}
}

#endif
