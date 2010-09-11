#include "speed_to_pitch.hpp"
#include "../math/sawtooth.hpp"
#include <algorithm>

sge::audio::scalar
insula::vehicle::speed_to_pitch(
	physics::scalar const s)
{
	return 
		math::sawtooth(
			s,
			static_cast<physics::scalar>(
				100));
	/*
	if (s <= static_cast<physics::scalar>(60))
		return 
			static_cast<sge::audio::scalar>(s) / 
			static_cast<sge::audio::scalar>(60);
	if (s <= static_cast<physics::scalar>(180))
		return 
			static_cast<sge::audio::scalar>(s - static_cast<sge::audio::scalar>(60)) / 
			static_cast<sge::audio::scalar>(180 - 60);
	if (s <= static_cast<physics::scalar>(150))
		return 
			static_cast<sge::audio::scalar>(s - static_cast<sge::audio::scalar>(180)) / 
			static_cast<sge::audio::scalar>(150 - 180);
	if (s <= static_cast<physics::scalar>(400))
		return 
			static_cast<sge::audio::scalar>(s - static_cast<sge::audio::scalar>(150)) / 
			static_cast<sge::audio::scalar>(400 - 150);
	return
		std::min(
			static_cast<sge::audio::scalar>(s - static_cast<sge::audio::scalar>(400)) / 
			static_cast<sge::audio::scalar>(50),
			static_cast<sge::audio::scalar>(1.0));
*/
}
