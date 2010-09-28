#ifndef INSULA_TURN_TIMER_HPP_INCLUDED
#define INSULA_TURN_TIMER_HPP_INCLUDED

#include <fcppt/string.hpp>
#include <chrono>

namespace insula
{
/**
	This class is (obviously) used to time one game iteration, but it
	can be used for different purposes, too.

	Note that this class is copyable. I don't see a reason why not.
*/
class turn_timer
{
public:
	explicit 
	turn_timer(
		std::chrono::milliseconds const &remaining_time);

	void
	add_to_remaining(
		std::chrono::milliseconds const &);

	std::chrono::milliseconds const
	milliseconds_total() const;

	std::chrono::milliseconds const
	milliseconds_remaining() const;

	bool
	expired() const;
private:
	typedef 
	std::chrono::high_resolution_clock 
	clock;

	typedef
	clock::time_point
	time_point;

	typedef
	clock::duration
	duration;
	
	time_point start_,end_;
};
}

#endif
