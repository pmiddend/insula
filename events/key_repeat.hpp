#ifndef INSULA_EVENTS_KEY_REPEAT_HPP_INCLUDED
#define INSULA_EVENTS_KEY_REPEAT_HPP_INCLUDED

#include <sge/input/key_type.hpp>
#include <boost/statechart/event.hpp>

namespace insula
{
namespace events
{
class key_repeat
:
	public boost::statechart::event<key_repeat>
{
public:
	explicit
	key_repeat(
		sge::input::key_type const &);

	sge::input::key_type const &
	pair() const;
private:
	sge::input::key_type k_;
};
}
}

#endif
