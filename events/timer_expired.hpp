#ifndef INSULA_EVENTS_TIMER_EXPIRED_HPP_INCLUDED
#define INSULA_EVENTS_TIMER_EXPIRED_HPP_INCLUDED

#include <boost/statechart/event.hpp>

namespace insula
{
namespace events
{
class timer_expired
:
	public boost::statechart::event<timer_expired>
{
};
}
}

#endif
