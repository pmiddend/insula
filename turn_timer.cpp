#include "turn_timer.hpp"
#include <fcppt/assert.hpp>
#include <fcppt/format.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/mod.hpp>
#include <chrono>

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

	typedef duration::rep rep;
	typedef std::make_unsigned<duration::rep>::type urep;

	rep const 
		minutes = 
			static_cast<rep>(
				d.count() / static_cast<rep>(1000*60)),
		residue =
			static_cast<rep>(
				fcppt::math::mod(
					static_cast<urep>(
						d.count()),
					static_cast<urep>(
						1000*60))),
		seconds = 
			static_cast<rep>(
				residue / static_cast<rep>(1000)),
		milliseconds = 
			static_cast<rep>(
				fcppt::math::mod(
					static_cast<urep>(residue),
					static_cast<urep>(1000)));
			
	return 
		(fcppt::format(FCPPT_TEXT("%1$u:%2$02u:%3$02u")) 
			% minutes
			% seconds
			% static_cast<rep>(milliseconds/static_cast<rep>(10))).str();
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
	end_ = clock::now();
}
