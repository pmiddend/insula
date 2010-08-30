#include "milliseconds_to_string.hpp"
#include <fcppt/format.hpp>
#include <fcppt/math/mod.hpp>
#include <fcppt/text.hpp>
#include <chrono>
#include <type_traits>

fcppt::string const
insula::milliseconds_to_string(
	std::chrono::milliseconds const &d)
{
	typedef std::chrono::milliseconds::rep rep;
	typedef std::make_unsigned<std::chrono::milliseconds::rep>::type urep;

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
