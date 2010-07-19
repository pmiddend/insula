#include "weights.hpp"

insula::textures::weights::weights(
	weight_sequence const &_sequence,
	weight const _gradient)
:
	sequence_(
		_sequence),
	gradient_(
		_gradient)
{
}

insula::textures::weights::weight_sequence const &
insula::textures::weights::sequence() const
{
	return sequence_;
}

insula::textures::weight
insula::textures::weights::gradient() const
{
	return gradient_;
}
