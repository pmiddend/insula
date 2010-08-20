#ifndef INSULA_EVENTS_RENDER_HPP_INCLUDED
#define INSULA_EVENTS_RENDER_HPP_INCLUDED

#include <boost/statechart/event.hpp>

namespace insula
{
namespace events
{
class render
:
	public boost::statechart::event<render>
{
public:
	explicit
	render();
};
}
}

#endif
