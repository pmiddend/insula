#ifndef INSULA_EVENTS_NUGGETS_EMPTY_HPP_INCLUDED
#define INSULA_EVENTS_NUGGETS_EMPTY_HPP_INCLUDED

#include <boost/statechart/event.hpp>

namespace insula
{
namespace events
{
class nuggets_empty
:
	public boost::statechart::event<nuggets_empty>
{
};
}
}

#endif
