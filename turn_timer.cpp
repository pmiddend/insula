#include "turn_timer.hpp"
#include "milliseconds_to_string.hpp"
#include <fcppt/assert.hpp>
#include <chrono>

insula::turn_timer::turn_timer(
	std::chrono::milliseconds const &remaining_time)
:
	start_(
		clock::now()),
	end_(
		start_ + remaining_time)
{
}

bool
insula::turn_timer::expired() const
{
	return (clock::now() - end_).count() >= 0;
}

void
insula::turn_timer::add_to_remaining(
	std::chrono::milliseconds const &ms)
{
	end_ += ms;
}

std::chrono::milliseconds const
insula::turn_timer::milliseconds_total() const
{
	return 
		std::chrono::duration_cast<std::chrono::milliseconds>(
			clock::now() - start_);
}

std::chrono::milliseconds const
insula::turn_timer::milliseconds_remaining() const
{
	return 
		std::chrono::duration_cast<std::chrono::milliseconds>(
			end_ - clock::now());
}
