#ifndef INSULA_EVENTS_KEY_HPP_INCLUDED
#define INSULA_EVENTS_KEY_HPP_INCLUDED

#include <sge/input/key_pair.hpp>
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
		sge::input::key_pair const &);

	sge::input::key_pair const &
	pair() const;
private:
	sge::input::key_pair k_;
};
}
}

#endif
