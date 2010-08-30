#ifndef INSULA_TURN_TIMER_HPP_INCLUDED
#define INSULA_TURN_TIMER_HPP_INCLUDED

#include <fcppt/string.hpp>
#include <fcppt/optional.hpp>
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
	turn_timer();

	fcppt::string const
	string();

	void
	start();

	void 
	stop();

	std::chrono::milliseconds const
	milliseconds() const;
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
	
	fcppt::optional<time_point> start_,end_;
};
}

#endif
