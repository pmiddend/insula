#include "turn_timer.hpp"
#include <fcppt/assert.hpp>

insula::turn_timer::turn_timer()
:
	start_(),
	end_()
{
}

fcppt::string const
insula::turn_timer::string()
{
	// Due to the asserts below, there must be either
	// - no start, no end
	// - start, but no end
	// - start and end
	std::chrono::milliseconds const d = 
		std::chrono::duration_cast<std::chrono::milliseconds>(
			start_ 
			?
				end_
				?
					*end_ - *start_
				:
					clock::now() - *start_
			:
				duration());

	return fcppt::string();
	/*
	typedef std::chrono::rep rep;

	rep const 
		minutes = 
			static_cast<rep>(d.count() / static_cast<rep>(1000) / static_cast<rep>(60)),
		seconds = 
			static_cast<rep>(d.count() / static_cast<rep>(1000)),
		millisecds = 
			

	return 
		(fcppt::format(FCPPT_TEXT("%1:%2:%3")) 
			%  
			% 
			% );*/
}

void
insula::turn_timer::start()
{
	FCPPT_ASSERT(
		!start_);
	start_ = clock::now();
}

void 
insula::turn_timer::stop()
{
	FCPPT_ASSERT(
		start_ && !end_);
	start_ = clock::now();
}
