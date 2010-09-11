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
}
